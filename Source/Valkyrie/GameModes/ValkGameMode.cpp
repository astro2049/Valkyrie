// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkGameMode.h"

AValkGameMode::AValkGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AValkGameMode::ScheduleReturnToMainMenu()
{
	if (GetWorldTimerManager().IsTimerActive(myReturnTimerHandle)) {
		return;
	}

	GetWorldTimerManager().SetTimer(
		myReturnTimerHandle,
		this,
		&AValkGameMode::ReturnPlayersToMainMenu,
		myPostMatchDelay,
		false
	);
}

void AValkGameMode::ReturnPlayersToMainMenu() const
{
	if (UWorld* const world = GetWorld()) {
		world->ServerTravel(TEXT("/Game/MainMenu/MainMenu"));
	}
}
