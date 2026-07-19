// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
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

	void Fire();
	void Reload();
	void EquipPrimaryGun();
	void EquipSecondaryGun();

	AGunActor* GetCurrentGunActor() const;
	bool IsReloading() const { return myIsReloading; }
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	EValkWeaponSlot GetCurrentSlot() const { return myCurrentSlot; }

private:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_TraceFire(FVector aTraceStart, FVector aTraceDirection);
	UFUNCTION(Server, Reliable)
	void Server_Reload();
	UFUNCTION(Server, Reliable)
	void Server_EquipGun(EValkWeaponSlot aWeaponSlot);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayFirePresentation(AGunActor* aGunActor);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayReloadPresentation(AGunActor* aGunActor);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayHitPresentation(FVector aHitPoint, const FVector aHitNormal);
	UFUNCTION()
	void OnRep_GunState();

	void SpawnGunActors();
	void SetCurrentGun(EValkWeaponSlot aWeaponSlot);
	void UpdateGunVisibility() const;
	void CancelReload();
	void FinishReload();

	FTimerHandle myReloadTimerHandle;

	UPROPERTY(ReplicatedUsing=OnRep_GunState)
	TObjectPtr<AGunActor> myPrimaryGunActor{nullptr};
	UPROPERTY(ReplicatedUsing=OnRep_GunState)
	TObjectPtr<AGunActor> mySecondaryGunActor{nullptr};
	UPROPERTY(ReplicatedUsing=OnRep_GunState)
	EValkWeaponSlot myCurrentSlot{EValkWeaponSlot::Primary};
	UPROPERTY(Replicated)
	bool myIsReloading{false};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	bool myDrawDebugTrace{true};

	// primary and secondary gun types
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<AGunActor> myPrimaryGunType;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<AGunActor> mySecondaryGunType;
	
	// on hit
	UPROPERTY(EditDefaultsOnly, Category="Apex")
	TObjectPtr<UNiagaraSystem> myImpactVFX;
};
