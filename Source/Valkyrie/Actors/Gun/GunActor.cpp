// Fill out your copyright notice in the Description page of Project Settings.


#include "GunActor.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AGunActor::AGunActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	myRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = myRootComponent;
	myMuzzleArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleArrowComponent"));
	myMuzzleArrowComponent->SetupAttachment(RootComponent);
}

void AGunActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && myGunDataAsset) {
		myAmmoInMag = FMath::Max(myGunDataAsset->myMagazineSize, 1);
		myReserveAmmo = FMath::Max(myGunDataAsset->myInitialReserveAmmo, 0);
		myFireInterval = myGunDataAsset->myRPM > 0.f ? 60.f / myGunDataAsset->myRPM : 0.f;
		myLastFiredTime = -1.f;
	}
}

void AGunActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGunActor, myAmmoInMag);
	DOREPLIFETIME(AGunActor, myReserveAmmo);
}

bool AGunActor::CanFire() const
{
	if (const UWorld* const world = GetWorld()) {
		return myAmmoInMag > 0 && world->GetTimeSeconds() - myLastFiredTime >= myFireInterval;
	}
	return false;
}

void AGunActor::ConsumeAmmo()
{
	if (const UWorld* const world = GetWorld()) {
		myLastFiredTime = world->GetTimeSeconds();
		myAmmoInMag = FMath::Max(myAmmoInMag - 1, 0);
	}
}

bool AGunActor::CanReload() const
{
	bool canReload = false;
	if (myGunDataAsset) {
		canReload = myAmmoInMag < GetMagazineSize() && myReserveAmmo > 0;
	}
	return canReload;
}

void AGunActor::ApplyReloadAmmo()
{
	const int32 ammoToLoad = FMath::Min(GetMagazineSize() - myAmmoInMag, myReserveAmmo);
	myAmmoInMag += ammoToLoad;
	myReserveAmmo -= ammoToLoad;
}

void AGunActor::PlayFirePresentation() const
{
	if (myMuzzleArrowComponent) {
		const FTransform muzzleTransform = myMuzzleArrowComponent->GetComponentTransform();
		if (myFireSound) {
			UGameplayStatics::PlaySoundAtLocation(this, myFireSound, muzzleTransform.GetLocation());
		}
		if (myMuzzleFlashVFX) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				myMuzzleFlashVFX,
				muzzleTransform.GetLocation(),
				muzzleTransform.GetRotation().Rotator(),
				muzzleTransform.GetScale3D()
			);
		}
	}
}

void AGunActor::PlayReloadPresentation() const
{
	if (myReloadSound) {
		UGameplayStatics::PlaySoundAtLocation(this, myReloadSound, GetActorLocation());
	}
}
