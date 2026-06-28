// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/PVP/PVPGameMode.h"
#include "TDMGameMode.generated.h"

UCLASS()
class VALKYRIE_API ATDMGameMode : public APVPGameMode
{
	GENERATED_BODY()

public:
	ATDMGameMode();

protected:
	virtual void HandleModePlayerKilled(AController* aVictimController, AController* aKillerController) override;
};
