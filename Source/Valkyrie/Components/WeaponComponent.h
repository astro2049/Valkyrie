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
	UFUNCTION(Server, Reliable)
	void Server_Reload();
	UFUNCTION(Server, Reliable)
	void Server_EquipGun(EValkWeaponSlot aWeaponSlot);

	AGunActor* GetCurrentGunActor() const;
	bool IsReloading() const { return myIsReloading; }
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	EValkWeaponSlot GetCurrentSlot() const { return myCurrentSlot; }

private:
	virtual void BeginPlay() override;
	void SpawnGunActors();
	void AttachGun(AGunActor* aGunActor) const;
	void SetCurrentGun(EValkWeaponSlot aWeaponSlot);
	UFUNCTION()
	void OnRep_UpdateGunVisibility() const;

	UFUNCTION(Server, Reliable)
	void Server_TraceFire(FVector aTraceStart, FVector aTraceDirection);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayHitPresentation(FVector aHitPoint, const FVector aHitNormal);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayReloadPresentation(AGunActor* aGunActor);
	void CancelReload();
	void FinishReload();

	FTimerHandle myReloadTimerHandle;

	UPROPERTY(Replicated)
	TObjectPtr<AGunActor> myPrimaryGunActor{nullptr};
	UPROPERTY(Replicated)
	TObjectPtr<AGunActor> mySecondaryGunActor{nullptr};
	UPROPERTY(ReplicatedUsing=OnRep_UpdateGunVisibility)
	EValkWeaponSlot myCurrentSlot{EValkWeaponSlot::Primary};
	UPROPERTY(Replicated)
	bool myIsReloading{false};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myTraceDistance{10000.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	bool myDrawDebugTrace{true};

	// primary and secondary gun types
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<AGunActor> myPrimaryGunType;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<AGunActor> mySecondaryGunType;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	FName myHandSocketName{"HandGrip_R"};

	// on hit
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UNiagaraSystem> myImpactVFX;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<USoundBase> myHitSoundCue;
};
