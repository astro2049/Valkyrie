// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/PVP/PVPGameMode.h"
#include "KillConfirmedGameMode.generated.h"

class AKillConfirmedTag;

UCLASS()
class VALKYRIE_API AKillConfirmedGameMode : public APVPGameMode
{
	GENERATED_BODY()

public:
	AKillConfirmedGameMode();
	bool ConfirmTag(const APawn* aPlayerPawn);

protected:
	virtual void HandleModePlayerKilled(AController* aVictimController, AController* aKillerController) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<AKillConfirmedTag> myTagClass;
};
