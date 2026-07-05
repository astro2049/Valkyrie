// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetExtractionData.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameState.h"

FValkExtractionData UUINode_GetExtractionData::GetExtractionData(APlayerController* const aPlayerController)
{
	FValkExtractionData data;
	if (aPlayerController) {
		if (const UWorld* const world = aPlayerController->GetWorld()) {
			if (const AExtractionGameState* const gameState = world->GetGameState<AExtractionGameState>()) {
				data.myCombatSliceState = gameState->GetCombatSliceState();
				data.myDefenseTimeRemaining = gameState->GetDefenseTimeRemaining();
			}
		}
	}
	return data;
}
