// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPScoreboardViewModel.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/GameModes/PVP/PVPGameState.h"
#include "Valkyrie/GameModes/PVP/Player/PVPPlayerState.h"
#include "Valkyrie/GameModes/PVP/PVPTypes.h"

void UPVPScoreboardViewModel::RefreshData()
{
	myScoreboardData = {};
	BuildScoreText(myScoreboardData.myTeamAScoreText, myScoreboardData.myTeamBScoreText);
	const APVPGameState* const gameState = GetPVPGameState();
	if (!gameState) {
		return;
	}

	for (const APlayerState* const playerState : gameState->PlayerArray) {
		const APVPPlayerState* const pvpPlayerState = Cast<APVPPlayerState>(playerState);
		FPVPScoreboardRowData rowData;
		if (!pvpPlayerState || !BuildRowData(playerState, rowData)) {
			continue;
		}

		if (pvpPlayerState->GetTeamId() == ValkTeamId::TeamA) {
			myScoreboardData.myTeamARows.Add(MoveTemp(rowData));
		} else if (pvpPlayerState->GetTeamId() == ValkTeamId::TeamB) {
			myScoreboardData.myTeamBRows.Add(MoveTemp(rowData));
		}
	}

	auto sortRows = [](const FPVPScoreboardRowData& aLeft, const FPVPScoreboardRowData& aRight) {
		if (aLeft.myKills != aRight.myKills) {
			return aLeft.myKills > aRight.myKills;
		}
		if (aLeft.myDeaths != aRight.myDeaths) {
			return aLeft.myDeaths < aRight.myDeaths;
		}
		return aLeft.myPlayerNameText.ToString() < aRight.myPlayerNameText.ToString();
	};
	myScoreboardData.myTeamARows.Sort(sortRows);
	myScoreboardData.myTeamBRows.Sort(sortRows);
}

APVPGameState* UPVPScoreboardViewModel::GetPVPGameState() const
{
	const APlayerController* const playerController = GetPlayerController();
	const UWorld* const world = playerController ? playerController->GetWorld() : nullptr;
	return world ? world->GetGameState<APVPGameState>() : nullptr;
}
