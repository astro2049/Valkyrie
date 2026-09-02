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
	void FinishMatch(EValkTeamId aTeamId);
	void ReturnPlayersToMainMenu() const;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	int32 myScoreLimit{10};

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myPostMatchDelay{5.f};
	FTimerHandle myReturnToMainMenuTimerHandle;
};
