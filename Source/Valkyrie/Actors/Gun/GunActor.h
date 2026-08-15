// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunDataAsset.h"
#include "NiagaraSystem.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "GunActor.generated.h"

class UGunDataAsset;
class UParticleSystem;
class USceneComponent;
class USoundBase;
class ACharacter;
class UAnimMontage;

UCLASS()
class VALKYRIE_API AGunActor : public AActor
{
	GENERATED_BODY()

public:
	AGunActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool CanFire() const;
	void ConsumeAmmo();
	bool CanReload() const;
	void ApplyReloadAmmo();
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayFirePresentation();

	// Getters
	float GetDamage() const { return myGunDataAsset ? myGunDataAsset->myDamage : 0.f; }
	float GetFireInterval() const { return myFireInterval; }
	float GetReloadDuration() const { return myGunDataAsset ? myGunDataAsset->myReloadDuration : 0.f; }
	float GetBaseSpreadHalfAngleDegrees() const { return myGunDataAsset ? myGunDataAsset->myBaseSpreadHalfAngleDegrees : 0.f; }
	float GetAimSpreadHalfAngleDegrees() const { return myGunDataAsset ? myGunDataAsset->myAimSpreadHalfAngleDegrees : 0.f; }
	float GetMaxMoveSpreadHalfAngleDegrees() const { return myGunDataAsset ? myGunDataAsset->myMaxMoveSpreadHalfAngleDegrees : 0.f; }
	float GetSpreadInterpSpeed() const { return myGunDataAsset ? myGunDataAsset->mySpreadInterpSpeed : 0.f; }
	float GetFireSpreadPerShotDegrees() const { return myGunDataAsset ? myGunDataAsset->myFireSpreadPerShotDegrees : 0.f; }
	float GetMaxFireSpreadOffsetDegrees() const { return myGunDataAsset ? myGunDataAsset->myMaxFireSpreadOffsetDegrees : 0.f; }
	float GetFireSpreadRecoverySpeedDegreesPerSecond() const { return myGunDataAsset ? myGunDataAsset->myFireSpreadRecoverySpeedDegreesPerSecond : 0.f; }
	UAnimMontage* GetReloadMontage() const { return myReloadMontage; }
	int32 GetAmmoInMag() const { return myAmmoInMag; }
	int32 GetReserveAmmo() const { return myReserveAmmo; }
	int32 GetMagazineSize() const { return myGunDataAsset ? myGunDataAsset->myMagazineSize : 0; }
	FVector GetMuzzleLocation() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UGunDataAsset> myGunDataAsset{nullptr};

	// Presentations
	// fire
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<USoundBase> myFireSound{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UAnimMontage> myReloadMontage{nullptr};
	// muzzle flash
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UNiagaraSystem> myMuzzleFlashVFX{nullptr};

	// States
	float myFireInterval{0.f};
	UPROPERTY(Replicated)
	int32 myAmmoInMag{0};
	UPROPERTY(Replicated)
	int32 myReserveAmmo{0};
	float myLastFiredTime{-1.f};

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<USceneComponent> myRootComponent{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UArrowComponent> myMuzzleArrowComponent{nullptr};
};
