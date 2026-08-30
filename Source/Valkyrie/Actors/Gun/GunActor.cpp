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
	check(myGunDataAsset);

	myFireInterval = 60.f / myGunDataAsset->myRPM;
	if (HasAuthority()) {
		myAmmoInMag = myGunDataAsset->myMagazineSize;
		myReserveAmmo = myGunDataAsset->myInitialReserveAmmo;
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
	return myAmmoInMag > 0 && GetWorld()->GetTimeSeconds() - myLastFiredTime >= myFireInterval;
}

void AGunActor::ConsumeAmmo()
{
	myLastFiredTime = GetWorld()->GetTimeSeconds();
	--myAmmoInMag;
}

void AGunActor::ApplyReloadAmmo()
{
	const int32 ammoToLoad = FMath::Min(myGunDataAsset->myMagazineSize - myAmmoInMag, myReserveAmmo);
	myAmmoInMag += ammoToLoad;
	myReserveAmmo -= ammoToLoad;
}

void AGunActor::Multicast_PlayFirePresentation_Implementation()
{
	PlayFirePresentation();
}

void AGunActor::PlayFirePresentation()
{
	const FTransform muzzleTransform = myMuzzleArrowComponent->GetComponentTransform();
	if (myFireSound) {
		UGameplayStatics::PlaySoundAtLocation(this, myFireSound, muzzleTransform.GetLocation());
	}
	if (myMuzzleFlashVFX) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			myMuzzleFlashVFX,
			muzzleTransform.GetLocation(),
			muzzleTransform.GetRotation().Rotator()
		);
	}
}
