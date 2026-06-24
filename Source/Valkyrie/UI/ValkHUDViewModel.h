// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ValkViewModel.h"
#include "ValkHUDViewModel.generated.h"

class UHealthComponent;
class UWeaponComponent;

struct FValkWeaponHUDData
{
	int32 myAmmoInMag{0};
	int32 myReserveAmmo{0};
	bool myShowAmmo{false};
	bool myIsReloading{false};
};

struct FValkHealthHUDData
{
	float myHealth{0.f};
	float myMaxHealth{0.f};
	bool myShowHealth{false};
	bool myIsDead{false};
};

UCLASS()
class VALKYRIE_API UValkHUDViewModel : public UValkViewModel
{
	GENERATED_BODY()

public:
	void BindToWeaponComponent(UWeaponComponent* aWeaponComponent);
	void BindToHealthComponent(UHealthComponent* aHealthComponent);

	const FValkWeaponHUDData& GetWeaponHUDData() const { return myWeaponHUDData; }
	const FValkHealthHUDData& GetHealthHUDData() const { return myHealthHUDData; }
	float GetHealthRatio() const;
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

	FValkWeaponHUDData myWeaponHUDData;
	FValkHealthHUDData myHealthHUDData;
	bool myShowInteractPrompt{false};
};
