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

protected:
	void ScheduleReturnToMainMenu();

private:
	void ReturnPlayersToMainMenu() const;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myPostMatchDelay{5.f};
	FTimerHandle myReturnTimerHandle;
};
