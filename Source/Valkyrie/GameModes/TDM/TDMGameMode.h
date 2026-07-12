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
	virtual void PostLogin(APlayerController* aNewPlayer) override;

protected:
	virtual void OnPlayerDied(AController* aKillerController, AController* aVictimController) override;

private:
	EValkTeamId GetBalancedTeamId() const;
	void HandlePlayerKilled(AController* aVictimController, AController* aKillerController);
	void EndTDMMatch(EValkTeamId aWinningTeamId);
	void RespawnPlayer(AController* aController);

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	int32 myScoreLimit{10};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myRespawnDelay{1.5f};
};
