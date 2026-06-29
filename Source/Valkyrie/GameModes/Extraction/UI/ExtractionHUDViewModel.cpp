// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionHUDViewModel.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameState.h"

void UExtractionHUDViewModel::RefreshData()
{
	Super::RefreshData();
	RefreshExtractionHUDData();
}

void UExtractionHUDViewModel::RefreshExtractionHUDData()
{
	const APlayerController* const playerController = GetPlayerController();
	const UWorld* const world = playerController ? playerController->GetWorld() : nullptr;
	const AExtractionGameState* const gameState = world ? world->GetGameState<AExtractionGameState>() : nullptr;
	if (gameState) {
		myExtractionHUDData.myObjectiveText = gameState->GetObjectiveText();
		myExtractionHUDData.myDefenseTimeRemaining = gameState->GetDefenseTimeRemaining();
		myExtractionHUDData.myShowDefenseTimer = gameState->ShouldShowDefenseTimer();
	} else {
		myExtractionHUDData = FExtractionHUDData{};
	}
}
