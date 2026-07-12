// Fill out your copyright notice in the Description page of Project Settings.


#include "GunActor.h"

#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AGunActor::AGunActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);
}

void AGunActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGunActor, myAmmoInMag);
	DOREPLIFETIME(AGunActor, myReserveAmmo);
}

void AGunActor::InitializeRuntimeState()
{
	if (myGunDataAsset) {
		myAmmoInMag = FMath::Max(myGunDataAsset->myMagazineSize, 1);
		myReserveAmmo = FMath::Max(myGunDataAsset->myInitialReserveAmmo, 0);
		myLastFiredTime = -1.f;
	}
}

void AGunActor::AttachToCharacter(ACharacter* const aCharacter)
{
	if (aCharacter) {
		if (USkeletalMeshComponent* const characterMesh = aCharacter->GetMesh()) {
			AttachToComponent(
				characterMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				myHandSocketName
			);
			SetActorRelativeTransform(myAttachTransform);
			SetOwner(aCharacter);
		}
	}
}

bool AGunActor::CanFire(const float aNow) const
{
	bool canFire = false;
	if (myGunDataAsset) {
		const float fireInterval = myGunDataAsset->myRPM > 0.f ? 60.f / myGunDataAsset->myRPM : 0.f;
		canFire = myAmmoInMag > 0 && aNow - myLastFiredTime >= fireInterval;
	}
	return canFire;
}

void AGunActor::ConsumeAmmo(const float aNow)
{
	myLastFiredTime = aNow;
	myAmmoInMag = FMath::Max(myAmmoInMag - 1, 0);
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
	const FTransform muzzleTransform = GetMuzzleTransform();
	if (myFireSound) {
		UGameplayStatics::PlaySoundAtLocation(this, myFireSound, muzzleTransform.GetLocation());
	}
	if (myMuzzleFlash) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), myMuzzleFlash, muzzleTransform);
	}
}

void AGunActor::PlayReloadPresentation() const
{
	if (myReloadSound) {
		UGameplayStatics::PlaySoundAtLocation(this, myReloadSound, GetActorLocation());
	}
}

FTransform AGunActor::GetMuzzleTransform() const
{
	FTransform muzzleTransform = GetActorTransform();
	TArray<UMeshComponent*> meshComponents;
	GetComponents(meshComponents);
	for (const UMeshComponent* const meshComponent : meshComponents) {
		if (meshComponent && meshComponent->DoesSocketExist(myMuzzleSocketName)) {
			muzzleTransform = meshComponent->GetSocketTransform(myMuzzleSocketName);
			break;
		}
	}
	return muzzleTransform;
}
