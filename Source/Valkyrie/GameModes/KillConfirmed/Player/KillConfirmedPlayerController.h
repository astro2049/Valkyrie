// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/PVP/Player/PVPPlayerController.h"
#include "KillConfirmedPlayerController.generated.h"

UCLASS()
class VALKYRIE_API AKillConfirmedPlayerController : public APVPPlayerController
{
	GENERATED_BODY()

protected:
	virtual UPVPHUDViewModel* CreateHUDViewModel() override;
	virtual UPVPScoreboardViewModel* CreateScoreboardViewModel() override;
};
