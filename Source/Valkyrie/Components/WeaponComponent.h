// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/ActorComponent.h"
#include "Valkyrie/Actors/Gun/GunActor.h"
#include "WeaponComponent.generated.h"

class USoundBase;

UENUM(BlueprintType)
enum class EValkWeaponSlot : uint8
{
	Primary,
	Secondary
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category="Valkyrie")
	AGunActor* GetCurrentGunActor() const;
	void EquipGun(const EValkWeaponSlot aWeaponSlot) { SetCurrentGun(aWeaponSlot); }
	void StartFiring();
	void StopFiring() { myIsFiring = false; }
	bool CanReload() const { return GetCurrentGunActor()->CanReload(); }
	float GetReloadDuration() const { return GetCurrentGunActor()->GetGunDataAsset()->myReloadDuration; }
	void ApplyReloadAmmo() { GetCurrentGunActor()->ApplyReloadAmmo(); }
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	bool IsReloading() const;
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	float GetSpreadAngle() const;
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	float GetBaseSpreadAngle() const { return GetCurrentGunActor()->GetGunDataAsset()->mySpreadBaseDegs; }
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	EValkWeaponSlot GetCurrentSlot() const { return myCurrentSlot; }

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SpawnGunActors();
	void AttachGun(AGunActor* aGunActor) const;
	void SetCurrentGun(EValkWeaponSlot aWeaponSlot);
	void TryFireOnce();
	void FireOnce();
	void UpdateSpread(float aDeltaTime);
	void AddFireSpread();
	UFUNCTION()
	void OnRep_UpdateGunVisibility();

	UFUNCTION(Client, Unreliable)
	void Client_AddFireSpread(EValkWeaponSlot aWeaponSlot);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayHitPresentation(FVector aHitPoint, const FVector aHitNormal);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayBulletTrailPresentation(FVector aTrailStart, FVector aTrailEnd);

	UPROPERTY(Replicated)
	TObjectPtr<AGunActor> myPrimaryGunActor{nullptr};
	UPROPERTY(Replicated)
	TObjectPtr<AGunActor> mySecondaryGunActor{nullptr};
	UPROPERTY(ReplicatedUsing=OnRep_UpdateGunVisibility)
	EValkWeaponSlot myCurrentSlot{EValkWeaponSlot::Primary};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myTraceDistance{10000.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	bool myDrawDebugTrace{false};
	float myFireSpreadAngle{0.f};
	bool myIsFiring{false};

	// primary and secondary gun types
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<AGunActor> myPrimaryGunType;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<AGunActor> mySecondaryGunType;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	FName myHandSocketName{"HandGrip_R"};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<USoundBase> myEmptyFireSound{nullptr};

	// on hit
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UNiagaraSystem> myBulletTrailVFX{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UNiagaraSystem> myImpactVFX;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<USoundBase> myHitSoundCue;
};
