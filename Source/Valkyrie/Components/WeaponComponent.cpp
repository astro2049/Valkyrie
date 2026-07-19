// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

#include "Valkyrie/Actors/Gun/GunActor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HealthComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/Player/ValkPlayerCharacter.h"

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
	DOREPLIFETIME(UWeaponComponent, myIsReloading);
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
	const UWorld* const world = GetWorld();
	const AActor* const owner = GetOwner();
	if (world && owner) {
		if (const UHealthComponent* const healthComponent = owner->FindComponentByClass<UHealthComponent>()) {
			if (AGunActor* const currentGunActor = GetCurrentGunActor()) {
				if (!healthComponent->IsDead() && !myIsReloading && !aTraceDirection.IsNearlyZero() && currentGunActor->CanFire()) {
					currentGunActor->ConsumeAmmo();

					// line trace
					const FVector start = aTraceStart;
					const FVector end = start + aTraceDirection.GetSafeNormal() * currentGunActor->GetTraceDistance();
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
					if (hasHit) {
						if (const APawn* const hitPawn = Cast<APawn>(hitResult.GetActor())) {
							if (UHealthComponent* health = hitPawn->FindComponentByClass<UHealthComponent>()) {
								AController* damageInstigator = nullptr;
								if (const APawn* const ownerPawn = Cast<APawn>(owner)) {
									damageInstigator = ownerPawn->GetController();
								}
								health->ApplyDamage(currentGunActor->GetDamage(), damageInstigator);
								Multicast_PlayHitPresentation(hitResult.ImpactPoint, hitResult.ImpactNormal);
							}
						}
					}

					Multicast_PlayFirePresentation(currentGunActor);
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
	}
}

void UWeaponComponent::Reload()
{
	Server_Reload();
}

void UWeaponComponent::Server_Reload_Implementation()
{
	const UWorld* const world = GetWorld();
	const AActor* const owner = GetOwner();
	if (world && owner) {
		if (const UHealthComponent* const ownerHealth = owner->FindComponentByClass<UHealthComponent>()) {
			AGunActor* const currentGunActor = GetCurrentGunActor();
			if (!ownerHealth->IsDead() &&
				!myIsReloading &&
				currentGunActor &&
				currentGunActor->CanReload()) {
				myIsReloading = true;
				Multicast_PlayReloadPresentation(currentGunActor);
				const float reloadDuration = currentGunActor->GetReloadDuration();
				if (reloadDuration > 0.f) {
					world->GetTimerManager().SetTimer(
						myReloadTimerHandle,
						this,
						&UWeaponComponent::FinishReload,
						reloadDuration,
						false
					);
				} else {
					FinishReload();
				}
			}
		}
	}
}

void UWeaponComponent::EquipPrimaryGun()
{
	Server_EquipGun(EValkWeaponSlot::Primary);
}

void UWeaponComponent::EquipSecondaryGun()
{
	Server_EquipGun(EValkWeaponSlot::Secondary);
}

void UWeaponComponent::Server_EquipGun_Implementation(const EValkWeaponSlot aWeaponSlot)
{
	if (aWeaponSlot != myCurrentSlot) {
		CancelReload();
		SetCurrentGun(aWeaponSlot);
	}
}

void UWeaponComponent::Multicast_PlayFirePresentation_Implementation(AGunActor* aGunActor)
{
	if (aGunActor) {
		aGunActor->PlayFirePresentation();
	}
}

void UWeaponComponent::Multicast_PlayReloadPresentation_Implementation(AGunActor* aGunActor)
{
	if (aGunActor) {
		aGunActor->PlayReloadPresentation();
	}
}

void UWeaponComponent::Multicast_PlayHitPresentation_Implementation(FVector aHitPoint, const FVector aHitNormal)
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

void UWeaponComponent::OnRep_GunState()
{
	UpdateGunVisibility();
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
					ownerCharacter->AttachGun(myPrimaryGunActor);
				}
			}
			if (mySecondaryGunType) {
				mySecondaryGunActor = world->SpawnActor<AGunActor>(mySecondaryGunType, FTransform::Identity, spawnParams);
				if (mySecondaryGunActor) {
					ownerCharacter->AttachGun(mySecondaryGunActor);
				}
			}
		}
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

void UWeaponComponent::SetCurrentGun(const EValkWeaponSlot aWeaponSlot)
{
	myCurrentSlot = aWeaponSlot;
	UpdateGunVisibility();
}

void UWeaponComponent::UpdateGunVisibility() const
{
	const AGunActor* const currentGunActor = GetCurrentGunActor();
	if (myPrimaryGunActor) {
		const bool shouldShowPrimary = currentGunActor == myPrimaryGunActor;
		myPrimaryGunActor->SetActorHiddenInGame(!shouldShowPrimary);
		myPrimaryGunActor->SetActorEnableCollision(shouldShowPrimary);
	}
	if (mySecondaryGunActor) {
		const bool shouldShowSecondary = currentGunActor == mySecondaryGunActor;
		mySecondaryGunActor->SetActorHiddenInGame(!shouldShowSecondary);
		mySecondaryGunActor->SetActorEnableCollision(shouldShowSecondary);
	}
}

void UWeaponComponent::CancelReload()
{
	if (myIsReloading) {
		if (const UWorld* const world = GetWorld()) {
			world->GetTimerManager().ClearTimer(myReloadTimerHandle);
		}
		myIsReloading = false;
	}
}

void UWeaponComponent::FinishReload()
{
	if (const AActor* const owner = GetOwner()) {
		if (AGunActor* const currentGunActor = GetCurrentGunActor()) {
			if (owner->HasAuthority() && myIsReloading) {
				currentGunActor->ApplyReloadAmmo();
				myIsReloading = false;
			}
		}
	}
}
