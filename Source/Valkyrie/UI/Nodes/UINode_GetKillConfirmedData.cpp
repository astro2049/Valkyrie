// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetKillConfirmedData.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/GameModes/PVP/KillConfirmed/KillConfirmedGameState.h"
#include "Valkyrie/Common/ValkTypes.h"

FValkKillConfirmedData UUINode_GetKillConfirmedData::GetKillConfirmedData(APlayerController* const aPlayerController)
{
	FValkKillConfirmedData data;
	data.myWinningTeamId = EValkTeamId::None;
	const UWorld* const world = aPlayerController ? aPlayerController->GetWorld() : nullptr;
	if (const AKillConfirmedGameState* const gameState = world ? world->GetGameState<AKillConfirmedGameState>() : nullptr) {
		data.myTeamAScore = gameState->GetTeamAConfirms();
		data.myTeamBScore = gameState->GetTeamBConfirms();
		data.myMatchEnded = gameState->HasMatchEnded();
		data.myWinningTeamId = gameState->GetWinningTeamId();
	}
	return data;
}
