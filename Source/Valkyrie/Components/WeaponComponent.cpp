// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

#include "Valkyrie/Actors/Gun/GunActor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HealthComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/Player/Character/ValkPlayerCharacter.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, myPrimaryGunActor);
	DOREPLIFETIME(UWeaponComponent, mySecondaryGunActor);
	DOREPLIFETIME(UWeaponComponent, myCurrentSlot);
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const AActor* const owner = GetOwner()) {
		if (owner->HasAuthority()) {
			SpawnGunActors();
			SetCurrentGun(EValkWeaponSlot::Primary);
		}
	}
}

void UWeaponComponent::SpawnGunActors()
{
	if (UWorld* const world = GetWorld()) {
		if (AValkPlayerCharacter* const ownerCharacter = Cast<AValkPlayerCharacter>(GetOwner())) {
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = ownerCharacter;
			if (myPrimaryGunType) {
				myPrimaryGunActor = world->SpawnActor<AGunActor>(myPrimaryGunType, FTransform::Identity, spawnParams);
				if (myPrimaryGunActor) {
					AttachGun(myPrimaryGunActor);
				}
			}
			if (mySecondaryGunType) {
				mySecondaryGunActor = world->SpawnActor<AGunActor>(mySecondaryGunType, FTransform::Identity, spawnParams);
				if (mySecondaryGunActor) {
					AttachGun(mySecondaryGunActor);
				}
			}
		}
	}
}

void UWeaponComponent::AttachGun(AGunActor* const aGunActor) const
{
	if (!aGunActor) {
		return;
	}

	if (const AValkPlayerCharacter* const ownerCharacter = Cast<AValkPlayerCharacter>(GetOwner())) {
		if (USkeletalMeshComponent* const characterMesh = ownerCharacter->GetMesh()) {
			aGunActor->AttachToComponent(
				characterMesh,
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				myHandSocketName
			);
		}
	}
}

void UWeaponComponent::SetCurrentGun(const EValkWeaponSlot aWeaponSlot)
{
	myCurrentSlot = aWeaponSlot;
	OnRep_UpdateGunVisibility();
}

void UWeaponComponent::OnRep_UpdateGunVisibility() const
{
	const bool shouldShowPrimaryGun = myCurrentSlot == EValkWeaponSlot::Primary;
	const bool shouldShowSecondaryGun = !shouldShowPrimaryGun;
	if (myPrimaryGunActor) {
		myPrimaryGunActor->SetActorHiddenInGame(!shouldShowPrimaryGun);
		myPrimaryGunActor->SetActorEnableCollision(!shouldShowPrimaryGun);
	}
	if (mySecondaryGunActor) {
		mySecondaryGunActor->SetActorHiddenInGame(!shouldShowSecondaryGun);
		mySecondaryGunActor->SetActorEnableCollision(!shouldShowSecondaryGun);
	}
}

void UWeaponComponent::Fire()
{
	if (const APawn* const ownerPawn = Cast<APawn>(GetOwner())) {
		if (const APlayerController* const playerController = Cast<APlayerController>(ownerPawn->GetController())) {
			if (const APlayerCameraManager* const playerCameraManager = playerController->PlayerCameraManager) {
				const FVector traceStart = playerCameraManager->GetCameraLocation();
				const FVector traceDirection = playerCameraManager->GetCameraRotation().Vector();
				Server_TraceFire(traceStart, traceDirection);
			}
		}
	}
}

void UWeaponComponent::Server_TraceFire_Implementation(const FVector aTraceStart, const FVector aTraceDirection)
{
	if (aTraceDirection.IsNearlyZero()) {
		return;
	}

	const UWorld* const world = GetWorld();
	const APawn* const owner = Cast<APawn>(GetOwner());
	AGunActor* const currentGunActor = GetCurrentGunActor();
	if (world && owner && currentGunActor) {
		if (!IsReloading() && currentGunActor->CanFire()) {
			// consume ammo
			currentGunActor->ConsumeAmmo();

			// line trace
			const FVector start = aTraceStart;
			const FVector end = start + aTraceDirection.GetSafeNormal() * myTraceDistance;
			FHitResult hitResult;
			FCollisionQueryParams params;
			params.AddIgnoredActor(owner);
			const bool hasHit = world->LineTraceSingleByChannel(
				hitResult,
				start,
				end,
				ECC_Visibility,
				params
			);
			Multicast_PlayBulletTrailPresentation(currentGunActor->GetMuzzleLocation(), hasHit ? hitResult.ImpactPoint : end);
			// if hit
			if (hasHit) {
				if (const APawn* const hitPawn = Cast<APawn>(hitResult.GetActor())) {
					if (UHealthComponent* health = hitPawn->FindComponentByClass<UHealthComponent>()) {
						AController* damageInstigator = nullptr;
						if (const APawn* const ownerPawn = Cast<APawn>(owner)) {
							damageInstigator = ownerPawn->GetController();
							if (AValkPlayerController* const ownerController = Cast<AValkPlayerController>(owner->GetController())) {
								// HUD: hit marker and sound (on shooter's side)
								ownerController->Client_PlayHitRepresentations();
							}
						}
						// apply damage
						health->ApplyDamage(currentGunActor->GetDamage(), damageInstigator);
						// blood mist VFX at impact point
						Multicast_PlayHitPresentation(hitResult.ImpactPoint, hitResult.ImpactNormal);
					}
				}
			}

			// debug trace
			if (myDrawDebugTrace) {
				DrawDebugLine(
					world,
					start,
					hasHit ? hitResult.ImpactPoint : end,
					hasHit ? FColor::Green : FColor::Red,
					false,
					1.0f,
					0,
					1.5f
				);
			}
		}
	}
}

void UWeaponComponent::Multicast_PlayHitPresentation_Implementation(const FVector aHitPoint, const FVector aHitNormal)
{
	if (myImpactVFX) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			myImpactVFX,
			aHitPoint,
			aHitNormal.Rotation()
		);
	}
}

void UWeaponComponent::CancelReload()
{
	if (const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetOwner())) {
		FGameplayTagContainer abilityTags;
		abilityTags.AddTag(AbilityTags::Ability_Reload);
		playerCharacter->GetAbilitySystemComponent()->CancelAbilities(&abilityTags);
	}
}

void UWeaponComponent::Server_EquipGun_Implementation(const EValkWeaponSlot aWeaponSlot)
{
	if (aWeaponSlot != myCurrentSlot) {
		CancelReload();
		SetCurrentGun(aWeaponSlot);
	}
}

AGunActor* UWeaponComponent::GetCurrentGunActor() const
{
	if (myCurrentSlot == EValkWeaponSlot::Primary) {
		return myPrimaryGunActor;
	}
	if (myCurrentSlot == EValkWeaponSlot::Secondary) {
		return mySecondaryGunActor;
	}
	return nullptr;
}

bool UWeaponComponent::CanReload() const
{
	const AGunActor* const currentGunActor = GetCurrentGunActor();
	return currentGunActor && currentGunActor->CanReload();
}

float UWeaponComponent::GetReloadDuration() const
{
	const AGunActor* const currentGunActor = GetCurrentGunActor();
	return currentGunActor ? currentGunActor->GetReloadDuration() : 0.f;
}

void UWeaponComponent::ApplyReloadAmmo()
{
	if (AGunActor* const currentGunActor = GetCurrentGunActor()) {
		currentGunActor->ApplyReloadAmmo();
	}
}

bool UWeaponComponent::IsReloading() const
{
	if (const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetOwner())) {
		return playerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(AbilityTags::State_Reloading);
	}
	return false;
}

void UWeaponComponent::Multicast_PlayBulletTrailPresentation_Implementation(const FVector aTrailStart, const FVector aTrailEnd)
{
	if (myBulletTrailVFX) {
		if (UNiagaraComponent* const trailComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			myBulletTrailVFX,
			aTrailStart,
			FRotator::ZeroRotator,
			FVector::OneVector,
			true,
			false
		)) {
			trailComponent->SetVariablePosition(TEXT("User.Beam End"), aTrailEnd);
			trailComponent->Activate();
		}
	}
}
