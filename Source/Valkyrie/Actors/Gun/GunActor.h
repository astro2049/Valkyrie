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

UCLASS()
class VALKYRIE_API AGunActor : public AActor
{
	GENERATED_BODY()

public:
	AGunActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	bool CanFire(float aNow) const;
	void ConsumeAmmo(float aNow);
	bool CanReload() const;
	void ApplyReloadAmmo();
	void PlayFirePresentation() const;
	void PlayReloadPresentation() const;

	// Getters
	float GetDamage() const { return myGunDataAsset ? myGunDataAsset->myDamage : 0.f; }
	float GetTraceDistance() const { return myGunDataAsset ? myGunDataAsset->myTraceDistance : 0.f; }
	float GetReloadDuration() const { return myGunDataAsset ? myGunDataAsset->myReloadDuration : 0.f; }
	int32 GetAmmoInMag() const { return myAmmoInMag; }
	int32 GetReserveAmmo() const { return myReserveAmmo; }
	int32 GetMagazineSize() const { return myGunDataAsset ? myGunDataAsset->myMagazineSize : 0; }

private:
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UGunDataAsset> myGunDataAsset{nullptr};
	
	// Presentations
	// fire
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<USoundBase> myFireSound{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	// reload
	TObjectPtr<USoundBase> myReloadSound{nullptr};
	// muzzle flash
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UNiagaraSystem> myMuzzleFlashVFX{nullptr};

	// States
	UPROPERTY(Replicated)
	int32 myAmmoInMag{0};
	UPROPERTY(Replicated)
	int32 myReserveAmmo{0};
	float myLastFiredTime{-1.f};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> myRootComponent{nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UArrowComponent> myMuzzleArrowComponent{nullptr};
};
