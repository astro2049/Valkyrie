// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "ValkGameMode.generated.h"

UCLASS()
class VALKYRIE_API AValkGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AValkGameMode();
	virtual void PostLogin(APlayerController* aNewPlayer) override; // assign team id
	virtual AActor* ChoosePlayerStart_Implementation(AController* aPlayer) override;
	virtual void PlayerDied(AController* aKillerController, AController* aVictimController);

protected:
	void ReturnToMainMenuAfterDelay(); 	// return to main menu after match ended

private:
	void ReturnPlayersToMainMenu() const;
	FTimerHandle myReturnToMainMenuTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myPostMatchDelay{5.f};
	
	int32 myTeamAPlayerCount{0};
	int32 myTeamBPlayerCount{0};
};
