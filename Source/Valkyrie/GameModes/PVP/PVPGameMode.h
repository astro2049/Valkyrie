// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/ValkGameMode.h"
#include "PVPGameMode.generated.h"

UCLASS(Abstract)
class VALKYRIE_API APVPGameMode : public AValkGameMode
{
	GENERATED_BODY()

public:
	APVPGameMode();

	virtual void PostLogin(APlayerController* aNewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* aPlayer) override;
	virtual void RestartPlayer(AController* aNewPlayer) override;

protected:
	virtual void HandleModePlayerKilled(AController* aVictimController, AController* aKillerController) PURE_VIRTUAL(APVPGameMode::HandleModePlayerKilled, );
	void EndPVPMatch(int32 aWinningTeamId);
	int32 GetScoreLimit() const { return myScoreLimit; }

private:
	int32 GetBalancedTeamId() const;
	void BindPlayerDeath(AController* aController);
	void HandlePlayerDeath(AController* aKillerController, AController* aVictimController);
	void RespawnPlayer(AController* aController);
	void ReturnPlayersToMainMenu();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	int32 myScoreLimit{10};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myRespawnDelay{3.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myPostMatchDelay{5.f};
	FTimerHandle myReturnTimerHandle;
};
