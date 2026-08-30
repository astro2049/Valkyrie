// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "Valkyrie/GameModes/ValkGameMode.h"
#include "TDMGameMode.generated.h"

UCLASS()
class VALKYRIE_API ATDMGameMode : public AValkGameMode
{
	GENERATED_BODY()

public:
	ATDMGameMode();

protected:
	virtual void PlayerDied(AController* aKillerController, AController* aVictimController) override;

private:
	void HandlePlayerKilled(AController* aVictimController, AController* aKillerController);
	void EndTDMMatch(EValkTeamId aWinningTeamId);

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	int32 myScoreLimit{10};
};
