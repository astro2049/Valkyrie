// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/PVP/UI/PVPHUDViewModel.h"
#include "KillConfirmedHUDViewModel.generated.h"

UCLASS()
class VALKYRIE_API UKillConfirmedHUDViewModel : public UPVPHUDViewModel
{
	GENERATED_BODY()

protected:
	virtual void RefreshModeHUDData() override;
};
