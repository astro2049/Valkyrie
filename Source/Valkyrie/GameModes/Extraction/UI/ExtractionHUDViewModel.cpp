// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionHUDViewModel.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameState.h"

void UExtractionHUDViewModel::RefreshData()
{
	RefreshExtractionHUDData();
}

void UExtractionHUDViewModel::RefreshExtractionHUDData()
{
	const APlayerController* const playerController = GetPlayerController();
	const UWorld* const world = playerController ? playerController->GetWorld() : nullptr;
	const AExtractionGameState* const gameState = world ? world->GetGameState<AExtractionGameState>() : nullptr;
	if (gameState) {
		myExtractionHUDData.myCombatSliceState = gameState->GetCombatSliceState();
		myExtractionHUDData.myDefenseTimeRemaining = gameState->GetDefenseTimeRemaining();
	} else {
		myExtractionHUDData = FExtractionHUDData{};
	}
}
