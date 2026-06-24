// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	// ...
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

	myMagazineSize = FMath::Max(1, myMagazineSize);
	myAmmoInMag = FMath::Clamp(myAmmoInMag, 0, myMagazineSize);
	myReserveAmmo = FMath::Max(0, myReserveAmmo);
	myReloadDuration = FMath::Max(0.f, myReloadDuration);
	myIsReloading = false;
	BroadcastWeaponStateChanged();
}


// Called every frame
void UWeaponComponent::TickComponent(float aDeltaTime, ELevelTick aTickType, FActorComponentTickFunction* aThisTickFunction)
{
	Super::TickComponent(aDeltaTime, aTickType, aThisTickFunction);

	// ...
}

void UWeaponComponent::Fire()
{
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (!ownerPawn) {
		return;
	}

	APlayerController* playerController = Cast<APlayerController>(ownerPawn->GetController());
	if (!playerController || !playerController->PlayerCameraManager) {
		return;
	}

	const FVector traceStart = playerController->PlayerCameraManager->GetCameraLocation();
	const FVector traceDirection = playerController->PlayerCameraManager->GetActorForwardVector();

	if (AActor* owner = GetOwner(); owner && owner->HasAuthority()) {
		TryFire(traceStart, traceDirection);
	} else {
		Server_TryFire(traceStart, traceDirection);
	}
}

void UWeaponComponent::TryFire(FVector aTraceStart, FVector aTraceDirection)
{
	UWorld* world = GetWorld();
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (!world || !ownerPawn || myIsReloading) {
		return;
	}

	if (myAmmoInMag <= 0) {
		BroadcastWeaponStateChanged();
		return;
	}

	const float now = world->GetTimeSeconds();
	const float fireInterval = myRPM > 0.f ? 60.f / myRPM : 0.f;
	if (now - myLastFiredTime < fireInterval) {
		return;
	}

	if (aTraceDirection.IsNearlyZero()) {
		return;
	}

	myLastFiredTime = now;
	myAmmoInMag = FMath::Max(0, myAmmoInMag - 1);
	BroadcastWeaponStateChanged();

	const FVector start = aTraceStart;
	const FVector direction = aTraceDirection.GetSafeNormal();
	const FVector end = start + direction * myTraceDistance;
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(ownerPawn);
	const bool bHit = world->LineTraceSingleByChannel(
		hitResult,
		start,
		end,
		ECC_Visibility,
		params
	);

	if (myDrawDebugTrace) {
		DrawDebugLine(
			world,
			start,
			bHit ? hitResult.ImpactPoint : end,
			bHit ? FColor::Green : FColor::Red,
			false,
			1.0f,
			0,
			1.5f
		);
	}

	if (myFireSound) {
		UGameplayStatics::PlaySoundAtLocation(world, myFireSound, start);
	}

	if (bHit) {
		if (AActor* hitActor = hitResult.GetActor()) {
			if (UHealthComponent* health = hitActor->FindComponentByClass<UHealthComponent>()) {
				health->ApplyDamage(myDamage);
			}

			myOnHitActor.Broadcast(hitActor, myDamage);
		}
	}
}

void UWeaponComponent::Server_TryFire_Implementation(FVector aTraceStart, FVector aTraceDirection)
{
	TryFire(aTraceStart, aTraceDirection);
}

void UWeaponComponent::OnRep_WeaponState()
{
	BroadcastWeaponStateChanged();
}

void UWeaponComponent::StartReload()
{
	UWorld* world = GetWorld();
	if (!world || myIsReloading || myAmmoInMag >= myMagazineSize || myReserveAmmo <= 0) {
		return;
	}

	myIsReloading = true;
	BroadcastWeaponStateChanged();

	if (myReloadDuration <= 0.f) {
		FinishReload();
		return;
	}

	world->GetTimerManager().SetTimer(
		myReloadTimerHandle,
		this,
		&UWeaponComponent::FinishReload,
		myReloadDuration,
		false
	);
}

void UWeaponComponent::CancelReload()
{
	UWorld* world = GetWorld();
	if (world) {
		world->GetTimerManager().ClearTimer(myReloadTimerHandle);
	}

	if (myIsReloading) {
		myIsReloading = false;
		BroadcastWeaponStateChanged();
	}
}

void UWeaponComponent::FinishReload()
{
	if (!myIsReloading) {
		return;
	}

	const int32 ammoNeeded = FMath::Max(0, myMagazineSize - myAmmoInMag);
	const int32 ammoToLoad = FMath::Min(ammoNeeded, myReserveAmmo);

	myAmmoInMag += ammoToLoad;
	myReserveAmmo -= ammoToLoad;
	myIsReloading = false;

	BroadcastWeaponStateChanged();
}

void UWeaponComponent::BroadcastWeaponStateChanged()
{
	myOnWeaponStateChanged.Broadcast();
}
