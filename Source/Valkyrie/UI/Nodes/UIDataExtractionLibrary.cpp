// Fill out your copyright notice in the Description page of Project Settings.

#include "UIDataExtractionLibrary.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameState.h"

FValkExtractionData UUIDataExtractionLibrary::GetExtractionData(APlayerController* const aPlayerController)
{
	FValkExtractionData data;
	const UWorld* const world = aPlayerController ? aPlayerController->GetWorld() : nullptr;
	if (const AExtractionGameState* const gameState = world ? world->GetGameState<AExtractionGameState>() : nullptr) {
		data.myCombatSliceState = gameState->GetCombatSliceState();
		data.myDefenseTimeRemaining = gameState->GetDefenseTimeRemaining();
	}
	return data;
}

bool UUIDataExtractionLibrary::EqualEqual_ValkExtractionData(
	const FValkExtractionData& aLeft,
	const FValkExtractionData& aRight
)
{
	return aLeft == aRight;
}
