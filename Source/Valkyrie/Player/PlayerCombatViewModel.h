// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerCombatViewModel.generated.h"

class UWeaponComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerCombatViewModelChanged);

UCLASS(BlueprintType)
class VALKYRIE_API UPlayerCombatViewModel : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Combat ViewModel")
	void BindToWeapon(UWeaponComponent* aWeaponComponent);
	UFUNCTION(BlueprintCallable, Category="Combat ViewModel")
	void UnbindWeapon();
	UFUNCTION(BlueprintCallable, Category="Combat ViewModel")
	void RefreshFromWeapon();

	UFUNCTION(BlueprintPure, Category="Combat ViewModel|Ammo")
	int GetAmmoInMag() const { return myAmmoInMag; }
	UFUNCTION(BlueprintPure, Category="Combat ViewModel|Ammo")
	int GetReserveAmmo() const { return myReserveAmmo; }
	UFUNCTION(BlueprintPure, Category="Combat ViewModel|Ammo")
	bool ShowAmmo() const { return myShowAmmo; }
	UFUNCTION(BlueprintPure, Category="Combat ViewModel|Weapon")
	bool IsReloading() const { return myIsReloading; }

	UPROPERTY(BlueprintAssignable, Category="Combat ViewModel")
	FPlayerCombatViewModelChanged myOnViewModelChanged;

private:
	UFUNCTION()
	void HandleWeaponStateChanged();

	void BroadcastViewModelChanged();

	UPROPERTY()
	TObjectPtr<UWeaponComponent> myWeaponComponent{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat ViewModel|Ammo", meta=(AllowPrivateAccess="true"))
	int myAmmoInMag{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat ViewModel|Ammo", meta=(AllowPrivateAccess="true"))
	int myReserveAmmo{0};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat ViewModel|Ammo", meta=(AllowPrivateAccess="true"))
	bool myShowAmmo{false};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat ViewModel|Weapon", meta=(AllowPrivateAccess="true"))
	bool myIsReloading{false};
};
