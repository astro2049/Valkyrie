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
	void BindToWeaponComponent(UWeaponComponent* aWeaponComponent);
	void BindToHealthComponent(UHealthComponent* aHealthComponent);

	int32 GetAmmoInMag() const { return myAmmoInMag; }
	int32 GetReserveAmmo() const { return myReserveAmmo; }
	bool ShowAmmo() const { return myShowAmmo; }
	bool IsReloading() const { return myIsReloading; }
	float GetHealth() const { return myHealth; }
	float GetMaxHealth() const { return myMaxHealth; }
	float GetHealthRatio() const;
	bool ShowHealth() const { return myShowHealth; }
	bool IsDead() const { return myIsDead; }
	bool GetShowInteractPrompt() const { return myShowInteractPrompt; }
	void SetShowInteractPrompt(bool aShowInteractPrompt);

private:
	UFUNCTION()
	void HandleWeaponStateChanged();
	UFUNCTION()
	void HandleHealthChanged(float aHealth, float aMaxHealth);

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
	bool myShowInteractPrompt{false};
};
