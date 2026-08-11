// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AGunActor;

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

	AGunActor* GetCurrentGunActor() const;
	void EquipGun(EValkWeaponSlot aWeaponSlot);
	bool CanReload() const;
	float GetReloadDuration() const;
	void ApplyReloadAmmo();
	bool IsReloading() const;
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	EValkWeaponSlot GetCurrentSlot() const { return myCurrentSlot; }

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SpawnGunActors();
	void AttachGun(AGunActor* aGunActor) const;
	void SetCurrentGun(EValkWeaponSlot aWeaponSlot);
	UFUNCTION()
	void OnRep_UpdateGunVisibility() const;

	void TryFireOnce();
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

	// primary and secondary gun types
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<AGunActor> myPrimaryGunType;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<AGunActor> mySecondaryGunType;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	FName myHandSocketName{"HandGrip_R"};

	// on hit
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UNiagaraSystem> myBulletTrailVFX{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UNiagaraSystem> myImpactVFX;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<USoundBase> myHitSoundCue;
};
