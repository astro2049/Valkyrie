// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "ValkGameMode.generated.h"

class AValkPlayerState;

UCLASS()
class VALKYRIE_API AValkGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AValkGameMode();
	virtual void PostLogin(APlayerController* aNewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* aPlayer) override;
	virtual void PlayerDied(AController* aKillerController, AController* aVictimController);

protected:
	void FinishMatch();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie", meta=(ClampMin="1", ClampMax="2"))
	int32 myTeamCount{1};

private:
	void AssignTeam(AValkPlayerState& aPlayerState) const;
	void RespawnPlayer(AController* aController);
	void ReturnToMainMenuAfterDelay();
	void ReturnPlayersToMainMenu() const;
	FTimerHandle myReturnToMainMenuTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myPostMatchDelay{5.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myRespawnDelay{3.f};
};
