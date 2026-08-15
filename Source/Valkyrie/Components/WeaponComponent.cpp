// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

#include "Valkyrie/Actors/Gun/GunActor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
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

	SetComponentTickEnabled(true);
	if (const AActor* const owner = GetOwner()) {
		if (owner->HasAuthority()) {
			SpawnGunActors();
			SetCurrentGun(EValkWeaponSlot::Primary);
		}
	}
}

void UWeaponComponent::TickComponent(
	const float DeltaTime,
	const ELevelTick TickType,
	FActorComponentTickFunction* const ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetOwner())) {
		UpdateSpread(DeltaTime);
	}
}

void UWeaponComponent::UpdateSpread(const float aDeltaTime)
{
	const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetOwner());
	const AGunActor* const currentGunActor = GetCurrentGunActor();
	if (playerCharacter && currentGunActor) {
		float desiredSpreadHalfAngleDegrees = currentGunActor->GetAimSpreadHalfAngleDegrees();
		if (!playerCharacter->IsAiming()) {
			const float maxWalkSpeed = playerCharacter->GetCharacterMovement()->MaxWalkSpeed;
			const float speedRatio = maxWalkSpeed > 0.f
				? FMath::Clamp(playerCharacter->GetVelocity().Size2D() / maxWalkSpeed, 0.f, 1.f)
				: 0.f;
			desiredSpreadHalfAngleDegrees = FMath::Lerp(
				currentGunActor->GetBaseSpreadHalfAngleDegrees(),
				currentGunActor->GetMaxMoveSpreadHalfAngleDegrees(),
				speedRatio
			);
		}
		if (myCurrentBaseSpreadHalfAngleDegrees < 0.f) {
			myCurrentBaseSpreadHalfAngleDegrees = desiredSpreadHalfAngleDegrees;
		} else {
			myCurrentBaseSpreadHalfAngleDegrees = FMath::FInterpTo(
				myCurrentBaseSpreadHalfAngleDegrees,
				desiredSpreadHalfAngleDegrees,
				aDeltaTime,
				currentGunActor->GetSpreadInterpSpeed()
			);
		}
		myFireSpreadOffsetDegrees = FMath::FInterpConstantTo(
			myFireSpreadOffsetDegrees,
			0.f,
			aDeltaTime,
			currentGunActor->GetFireSpreadRecoverySpeedDegreesPerSecond()
		);
	}
}

void UWeaponComponent::AddFireSpread()
{
	if (const AGunActor* const currentGunActor = GetCurrentGunActor()) {
		myFireSpreadOffsetDegrees = FMath::Clamp(
			myFireSpreadOffsetDegrees + FMath::Max(currentGunActor->GetFireSpreadPerShotDegrees(), 0.f),
			0.f,
			FMath::Max(currentGunActor->GetMaxFireSpreadOffsetDegrees(), 0.f)
		);
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

void UWeaponComponent::OnRep_UpdateGunVisibility()
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

	myCurrentBaseSpreadHalfAngleDegrees = GetCurrentGunActor() ? GetBaseSpreadHalfAngleDegrees() : -1.f;
	myFireSpreadOffsetDegrees = 0.f;
}

void UWeaponComponent::PlayPredictedFire()
{
	if (AGunActor* const currentGunActor = GetCurrentGunActor()) {
		if (!IsReloading() && currentGunActor->GetAmmoInMag() > 0) {
			currentGunActor->PlayFirePresentation();
		}
	}
}

void UWeaponComponent::TryCommitFire()
{
	const UWorld* const world = GetWorld();
	const APawn* const owner = Cast<APawn>(GetOwner());
	AGunActor* const currentGunActor = GetCurrentGunActor();
	if (world && owner && currentGunActor) {
		const APlayerController* const playerController = Cast<APlayerController>(owner->GetController());
		if (playerController && !IsReloading() && currentGunActor->CanFire()) {
			FVector traceStart;
			FRotator traceRotation;
			playerController->GetPlayerViewPoint(traceStart, traceRotation);
			const FVector traceDirection = FMath::VRandCone(
				traceRotation.Vector(),
				FMath::DegreesToRadians(GetCurrentSpreadHalfAngleDegrees())
			);

			// consume ammo
			currentGunActor->ConsumeAmmo();
			currentGunActor->Multicast_PlayFirePresentation();
			AddFireSpread();
			Client_AddFireSpread(myCurrentSlot);

			// line trace
			const FVector start = traceStart;
			const FVector end = start + traceDirection * myTraceDistance;
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

float UWeaponComponent::GetFireInterval() const
{
	const AGunActor* const currentGunActor = GetCurrentGunActor();
	return currentGunActor ? currentGunActor->GetFireInterval() : 0.f;
}

void UWeaponComponent::Client_AddFireSpread_Implementation(const EValkWeaponSlot aWeaponSlot)
{
	if (const AActor* const owner = GetOwner(); owner && !owner->HasAuthority() && myCurrentSlot == aWeaponSlot) {
		AddFireSpread();
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

void UWeaponComponent::EquipGun(const EValkWeaponSlot aWeaponSlot)
{
	SetCurrentGun(aWeaponSlot);
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

float UWeaponComponent::GetBaseSpreadHalfAngleDegrees() const
{
	const AGunActor* const currentGunActor = GetCurrentGunActor();
	return currentGunActor ? currentGunActor->GetBaseSpreadHalfAngleDegrees() : 0.f;
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
		return playerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(AbilityTags::Ability_Reload);
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
