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
	void PlayFirePresentation();
	bool CanReload() const { return myAmmoInMag < GetMagazineSize() && myReserveAmmo > 0; }
	void ApplyReloadAmmo();
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayFirePresentation();

	// Getters
	float GetDamage() const { return myGunDataAsset->myDamage; }
	float GetFireInterval() const { return myFireInterval; }
	float GetReloadDuration() const { return myGunDataAsset->myReloadDuration; }
	float GetBaseSpreadHalfAngleDegrees() const { return myGunDataAsset->myBaseSpreadHalfAngleDegrees; }
	float GetAimSpreadHalfAngleDegrees() const { return myGunDataAsset->myAimSpreadHalfAngleDegrees; }
	float GetMaxMoveSpreadHalfAngleDegrees() const { return myGunDataAsset->myMaxMoveSpreadHalfAngleDegrees; }
	float GetSpreadInterpSpeed() const { return myGunDataAsset->mySpreadInterpSpeed; }
	float GetFireSpreadPerShotDegrees() const { return myGunDataAsset->myFireSpreadPerShotDegrees; }
	float GetMaxFireSpreadOffsetDegrees() const { return myGunDataAsset->myMaxFireSpreadOffsetDegrees; }
	float GetFireSpreadRecoverySpeedDegreesPerSecond() const { return myGunDataAsset->myFireSpreadRecoverySpeedDegreesPerSecond; }
	UAnimMontage* GetReloadMontage() const { return myReloadMontage; }
	int32 GetAmmoInMag() const { return myAmmoInMag; }
	int32 GetReserveAmmo() const { return myReserveAmmo; }
	int32 GetMagazineSize() const { return myGunDataAsset->myMagazineSize; }
	FVector GetMuzzleLocation() const { return myMuzzleArrowComponent ? myMuzzleArrowComponent->GetComponentLocation() : GetActorLocation(); }

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
