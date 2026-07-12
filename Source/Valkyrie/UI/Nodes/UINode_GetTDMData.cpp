// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetTDMData.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "Valkyrie/GameModes/TDM/TDMGameState.h"

FValkTDMData UUINode_GetTDMData::GetTDMData(APlayerController* const aPlayerController)
{
	FValkTDMData data;
	data.myWinningTeamId = EValkTeamId::None;
	if (aPlayerController) {
		if (const UWorld* const world = aPlayerController->GetWorld()) {
			if (const ATDMGameState* const gameState = world->GetGameState<ATDMGameState>()) {
				data.myTeamAScore = gameState->GetTeamAKills();
				data.myTeamBScore = gameState->GetTeamBKills();
				data.myMatchEnded = gameState->HasMatchEnded();
				data.myWinningTeamId = gameState->GetWinningTeamId();
			}
		}
	}
	return data;
}
