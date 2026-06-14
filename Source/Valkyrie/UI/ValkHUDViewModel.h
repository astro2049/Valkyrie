// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ValkViewModel.h"
#include "ValkHUDViewModel.generated.h"

class UHealthComponent;
class UWeaponComponent;

UCLASS()
class VALKYRIE_API UValkHUDViewModel : public UValkViewModel
{
	GENERATED_BODY()

public:
	void BindToWeapon(UWeaponComponent* aWeaponComponent);
	void UnbindWeapon();
	void RefreshFromWeapon();
	void BindToHealth(UHealthComponent* aHealthComponent);
	void UnbindHealth();
	void RefreshFromHealth();

	int32 GetAmmoInMag() const { return myAmmoInMag; }
	int32 GetReserveAmmo() const { return myReserveAmmo; }
	bool ShowAmmo() const { return myShowAmmo; }
	bool IsReloading() const { return myIsReloading; }
	float GetHealth() const { return myHealth; }
	float GetMaxHealth() const { return myMaxHealth; }
	float GetHealthRatio() const;
	bool ShowHealth() const { return myShowHealth; }
	bool IsDead() const { return myIsDead; }

private:
	UFUNCTION()
	void HandleWeaponStateChanged();
	UFUNCTION()
	void HandleHealthChanged(float aHealth, float aMaxHealth);
	UFUNCTION()
	void HandleDeath();

	UPROPERTY()
	TObjectPtr<UWeaponComponent> myWeaponComponent{nullptr};
	UPROPERTY()
	TObjectPtr<UHealthComponent> myHealthComponent{nullptr};

	int32 myAmmoInMag{0};
	int32 myReserveAmmo{0};
	bool myShowAmmo{false};
	bool myIsReloading{false};
	float myHealth{0.f};
	float myMaxHealth{0.f};
	bool myShowHealth{false};
	bool myIsDead{false};
};
