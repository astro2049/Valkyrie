// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetTDMData.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "Valkyrie/GameModes/PVP/TDM/TDMGameState.h"

FValkTDMData UUINode_GetTDMData::GetTDMData(APlayerController* const aPlayerController)
{
	FValkTDMData data;
	data.myWinningTeamId = ValkTeamId::None;
	const UWorld* const world = aPlayerController ? aPlayerController->GetWorld() : nullptr;
	if (const ATDMGameState* const gameState = world ? world->GetGameState<ATDMGameState>() : nullptr) {
		data.myTeamAScore = gameState->GetTeamAKills();
		data.myTeamBScore = gameState->GetTeamBKills();
		data.myMatchEnded = gameState->HasMatchEnded();
		data.myWinningTeamId = gameState->GetWinningTeamId();
	}
	return data;
}
