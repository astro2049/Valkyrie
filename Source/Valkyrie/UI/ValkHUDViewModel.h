// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ValkViewModel.h"
#include "ValkHUDViewModel.generated.h"

class AValkPlayerPawn;

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
	void BindToPawn(AValkPlayerPawn* aPlayerPawn);

	const FValkWeaponHUDData& GetWeaponHUDData() const { return myWeaponHUDData; }
	const FValkHealthHUDData& GetHealthHUDData() const { return myHealthHUDData; }
	float GetHealthRatio() const { return myHealthHUDData.myMaxHealth > 0.f ? myHealthHUDData.myHealth / myHealthHUDData.myMaxHealth : 0.f; }
	bool GetShowInteractPrompt() const { return myShowInteractPrompt; }

private:
	void HandlePlayerPawnStateChanged();

	void RefreshPawnData();
	void RefreshWeaponHUDData();
	void RefreshHealthHUDData();
	void RefreshInteractionHUDData();

	TWeakObjectPtr<AValkPlayerPawn> myPlayerPawn;
	FValkWeaponHUDData myWeaponHUDData;
	FValkHealthHUDData myHealthHUDData;
	bool myShowInteractPrompt{false};
};
