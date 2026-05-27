// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "WeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FWeaponHitActor,
	AActor*, aHitActor,
	float, aDamage
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponStateChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float aDeltaTime, ELevelTick aTickType, FActorComponentTickFunction* aThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void StartReload();
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void CancelReload();

	UFUNCTION(BlueprintPure, Category="Weapon|Ammo")
	int GetAmmoInMag() const { return myAmmoInMag; }
	UFUNCTION(BlueprintPure, Category="Weapon|Ammo")
	int GetReserveAmmo() const { return myReserveAmmo; }
	UFUNCTION(BlueprintPure, Category="Weapon|Ammo")
	int GetMagazineSize() const { return myMagazineSize; }
	UFUNCTION(BlueprintPure, Category="Weapon")
	bool IsReloading() const { return myIsReloading; }

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FWeaponHitActor myOnHitActor;

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FWeaponStateChanged myOnWeaponStateChanged;

private:
	void FinishReload();
	void BroadcastWeaponStateChanged();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	float myDamage{25.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	float myRPM{600.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	float myTraceDistance{1000.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	USoundBase* myFireSound{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Reload", meta=(AllowPrivateAccess="true"))
	float myReloadDuration{2.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Debug", meta=(AllowPrivateAccess="true"))
	bool myDrawDebugTrace{true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Ammo", meta=(AllowPrivateAccess="true"))
	int myMagazineSize{30};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Ammo", meta=(AllowPrivateAccess="true"))
	int myAmmoInMag{30};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Ammo", meta=(AllowPrivateAccess="true"))
	int myReserveAmmo{90};

	float myLastFiredTime = -1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess="true"))
	bool myIsReloading{false};

	FTimerHandle myReloadTimerHandle;
};
