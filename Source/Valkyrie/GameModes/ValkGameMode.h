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
	virtual void PostLogin(APlayerController* aNewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* aPlayer) override;

	virtual void OnPlayerDied(AController* aKillerController, AController* aVictimController);

protected:
	void ScheduleReturnToMainMenu();

private:
	void ReturnPlayersToMainMenu() const;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myPostMatchDelay{5.f};
	FTimerHandle myReturnTimerHandle;
};
