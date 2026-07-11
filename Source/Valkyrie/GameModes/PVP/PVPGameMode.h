// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "Valkyrie/GameModes/ValkGameMode.h"
#include "PVPGameMode.generated.h"

UCLASS(Abstract)
class VALKYRIE_API APVPGameMode : public AValkGameMode
{
	GENERATED_BODY()

public:
	APVPGameMode();

	virtual void PostLogin(APlayerController* aNewPlayer) override;

protected:
	virtual void OnPlayerDied(AController* aKillerController, AController* aVictimController) override;
	virtual void HandleModePlayerKilled(AController* aVictimController, AController* aKillerController) PURE_VIRTUAL(APVPGameMode::HandleModePlayerKilled, );
	void EndPVPMatch(EValkTeamId aWinningTeamId);
	int32 GetScoreLimit() const { return myScoreLimit; }

private:
	EValkTeamId GetBalancedTeamId() const;
	void RespawnPlayer(AController* aController);

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	int32 myScoreLimit{10};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myRespawnDelay{1.5f};
};
