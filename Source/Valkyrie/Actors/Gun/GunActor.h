// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunDataAsset.h"
#include "GunActor.generated.h"

class UGunDataAsset;
class UParticleSystem;
class USoundBase;
class ACharacter;

UCLASS()
class VALKYRIE_API AGunActor : public AActor
{
	GENERATED_BODY()

public:
	AGunActor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitializeRuntimeState();
	void AttachToCharacter(ACharacter* aCharacter);
	bool CanFire(float aNow) const;
	void ConsumeAmmo(float aNow);
	bool CanReload() const;
	void ApplyReloadAmmo();
	void PlayFirePresentation() const;
	void PlayReloadPresentation() const;

	float GetDamage() const { return myGunDataAsset ? myGunDataAsset->myDamage : 0.f; }
	float GetTraceDistance() const { return myGunDataAsset ? myGunDataAsset->myTraceDistance : 0.f; }
	float GetReloadDuration() const { return myGunDataAsset ? myGunDataAsset->myReloadDuration : 0.f; }
	int32 GetAmmoInMag() const { return myAmmoInMag; }
	int32 GetReserveAmmo() const { return myReserveAmmo; }
	int32 GetMagazineSize() const { return myGunDataAsset ? myGunDataAsset->myMagazineSize : 0; }

private:
	FTransform GetMuzzleTransform() const;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UGunDataAsset> myGunDataAsset{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<USoundBase> myFireSound{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<USoundBase> myReloadSound{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UParticleSystem> myMuzzleFlash{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	FName myHandSocketName{"HandGrip_R"};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	FName myMuzzleSocketName{"muzzle"};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	FTransform myAttachTransform{FTransform::Identity};

	UPROPERTY(Replicated)
	int32 myAmmoInMag{0};
	UPROPERTY(Replicated)
	int32 myReserveAmmo{0};

	float myLastFiredTime{-1.f};
};
