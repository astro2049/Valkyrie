// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, myAmmoInMag);
	DOREPLIFETIME(UWeaponComponent, myReserveAmmo);
	DOREPLIFETIME(UWeaponComponent, myIsReloading);
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	myMagazineSize = FMath::Max(myMagazineSize, 1);
	myAmmoInMag = FMath::Clamp(myAmmoInMag, 0, myMagazineSize);
	myReserveAmmo = FMath::Max(myReserveAmmo, 0);
	myReloadDuration = FMath::Max(myReloadDuration, 0.f);
	myIsReloading = false;
	myFireInterval = myRPM > 0.f ? 60.f / myRPM : 0.f;
}

void UWeaponComponent::Fire()
{
	if (const APawn* const ownerPawn = Cast<APawn>(GetOwner())) {
		if (const APlayerController* const playerController = Cast<APlayerController>(ownerPawn->GetController())) {
			if (const APlayerCameraManager* const playerCameraManager = playerController->PlayerCameraManager) {
				const FVector traceStart = playerCameraManager->GetCameraLocation();
				const FVector traceDirection = playerCameraManager->GetActorForwardVector();

				if (!ownerPawn->HasAuthority()) {
					RPC_TraceFire(traceStart, traceDirection);
				} else {
					TraceFire(traceStart, traceDirection);
				}
			}
		}
	}
}

void UWeaponComponent::TraceFire(const FVector aTraceStart, const FVector aTraceDirection)
{
	const UWorld* const world = GetWorld();
	const AActor* const owner = GetOwner();
	const UHealthComponent* const ownerHealth = owner ? owner->FindComponentByClass<UHealthComponent>() : nullptr;
	if (!world || !owner || (ownerHealth && ownerHealth->IsDead()) || myIsReloading || myAmmoInMag <= 0 || aTraceDirection.IsNearlyZero()) {
		return;
	}

	const float now = world->GetTimeSeconds();
	if (now - myLastFiredTime >= myFireInterval) {
		myLastFiredTime = now;
		myAmmoInMag = FMath::Max(0, myAmmoInMag - 1);

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
		if (hasHit) {
			if (AActor* hitActor = hitResult.GetActor()) {
				if (UHealthComponent* health = hitActor->FindComponentByClass<UHealthComponent>()) {
					const APawn* const ownerPawn = Cast<APawn>(owner);
					health->ApplyDamage(myDamage, ownerPawn ? ownerPawn->GetController() : nullptr);
				}
			}
		}

		// sound
		if (myFireSound) {
			UGameplayStatics::PlaySoundAtLocation(world, myFireSound, start);
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

void UWeaponComponent::RPC_TraceFire_Implementation(const FVector aTraceStart, const FVector aTraceDirection)
{
	TraceFire(aTraceStart, aTraceDirection);
}

void UWeaponComponent::StartReload()
{
	if (const AActor* owner = GetOwner()) {
		if (!owner->HasAuthority()) {
			RPC_TryStartReload();
		} else {
			TryStartReload();
		}
	}
}

void UWeaponComponent::TryStartReload()
{
	const UWorld* const world = GetWorld();
	const AActor* const owner = GetOwner();
	const UHealthComponent* const ownerHealth = owner ? owner->FindComponentByClass<UHealthComponent>() : nullptr;
	if (!world || (ownerHealth && ownerHealth->IsDead()) || myIsReloading || myAmmoInMag >= myMagazineSize || myReserveAmmo <= 0) {
		return;
	}

	myIsReloading = true;
	if (myReloadDuration > 0.f) {
		world->GetTimerManager().SetTimer(
			myReloadTimerHandle,
			this,
			&UWeaponComponent::FinishReload,
			myReloadDuration,
			false
		);
	} else {
		FinishReload();
	}
}

void UWeaponComponent::RPC_TryStartReload_Implementation()
{
	TryStartReload();
}

void UWeaponComponent::FinishReload()
{
	if (myIsReloading) {
		const int32 ammoToLoad = FMath::Min(myMagazineSize - myAmmoInMag, myReserveAmmo);
		myAmmoInMag += ammoToLoad;
		myReserveAmmo -= ammoToLoad;
		myIsReloading = false;
	}
}
