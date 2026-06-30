// Fill out your copyright notice in the Description page of Project Settings.

#include "UIDataPVPLibrary.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/GameModes/KillConfirmed/KillConfirmedGameState.h"
#include "Valkyrie/GameModes/KillConfirmed/Player/KillConfirmedPlayerState.h"
#include "Valkyrie/GameModes/PVP/PVPGameState.h"
#include "Valkyrie/GameModes/PVP/Player/PVPPlayerState.h"
#include "Valkyrie/GameModes/PVP/PVPTypes.h"
#include "Valkyrie/GameModes/TDM/TDMGameState.h"
#include "Valkyrie/GameModes/TDM/Player/TDMPlayerState.h"

FValkPVPData UUIDataPVPLibrary::GetTDMData(APlayerController* const aPlayerController)
{
	FValkPVPData data;
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

FValkPVPData UUIDataPVPLibrary::GetKillConfirmedData(APlayerController* const aPlayerController)
{
	FValkPVPData data;
	data.myWinningTeamId = ValkTeamId::None;
	const UWorld* const world = aPlayerController ? aPlayerController->GetWorld() : nullptr;
	if (const AKillConfirmedGameState* const gameState = world ? world->GetGameState<AKillConfirmedGameState>() : nullptr) {
		data.myTeamAScore = gameState->GetTeamAConfirms();
		data.myTeamBScore = gameState->GetTeamBConfirms();
		data.myMatchEnded = gameState->HasMatchEnded();
		data.myWinningTeamId = gameState->GetWinningTeamId();
	}
	return data;
}

bool UUIDataPVPLibrary::EqualEqual_ValkPVPData(
	const FValkPVPData& aLeft,
	const FValkPVPData& aRight
)
{
	return aLeft == aRight;
}

void UUIDataPVPLibrary::GetPVPScoreboardData(
	APlayerController* const aPlayerController,
	int32& aTeamAScore,
	int32& aTeamBScore,
	bool& aShowConfirms,
	TArray<FValkScoreboardRowData>& someTeamARows,
	TArray<FValkScoreboardRowData>& someTeamBRows
)
{
	aTeamAScore = 0;
	aTeamBScore = 0;
	aShowConfirms = false;
	someTeamARows.Reset();
	someTeamBRows.Reset();

	const UWorld* const world = aPlayerController ? aPlayerController->GetWorld() : nullptr;
	const APVPGameState* const gameState = world ? world->GetGameState<APVPGameState>() : nullptr;
	if (!gameState) {
		return;
	}

	if (const AKillConfirmedGameState* const killConfirmedGameState = Cast<AKillConfirmedGameState>(gameState)) {
		aTeamAScore = killConfirmedGameState->GetTeamAConfirms();
		aTeamBScore = killConfirmedGameState->GetTeamBConfirms();
		aShowConfirms = true;
	} else if (const ATDMGameState* const tdmGameState = Cast<ATDMGameState>(gameState)) {
		aTeamAScore = tdmGameState->GetTeamAKills();
		aTeamBScore = tdmGameState->GetTeamBKills();
	}

	for (const APlayerState* const playerState : gameState->PlayerArray) {
		const APVPPlayerState* const pvpPlayerState = Cast<APVPPlayerState>(playerState);
		if (!pvpPlayerState) {
			continue;
		}

		FValkScoreboardRowData rowData;
		rowData.myPlayerName = pvpPlayerState->GetPlayerName();
		if (const AKillConfirmedPlayerState* const killConfirmedPlayerState = Cast<AKillConfirmedPlayerState>(playerState)) {
			rowData.myKills = killConfirmedPlayerState->GetKills();
			rowData.myDeaths = killConfirmedPlayerState->GetDeaths();
			rowData.myConfirms = killConfirmedPlayerState->GetConfirms();
		} else if (const ATDMPlayerState* const tdmPlayerState = Cast<ATDMPlayerState>(playerState)) {
			rowData.myKills = tdmPlayerState->GetKills();
			rowData.myDeaths = tdmPlayerState->GetDeaths();
		} else {
			continue;
		}

		if (pvpPlayerState->GetTeamId() == ValkTeamId::TeamA) {
			someTeamARows.Add(MoveTemp(rowData));
		} else if (pvpPlayerState->GetTeamId() == ValkTeamId::TeamB) {
			someTeamBRows.Add(MoveTemp(rowData));
		}
	}

	auto sortRows = [](const FValkScoreboardRowData& aLeft, const FValkScoreboardRowData& aRight) {
		if (aLeft.myKills != aRight.myKills) {
			return aLeft.myKills > aRight.myKills;
		}
		if (aLeft.myDeaths != aRight.myDeaths) {
			return aLeft.myDeaths < aRight.myDeaths;
		}
		return aLeft.myPlayerName < aRight.myPlayerName;
	};
	someTeamARows.Sort(sortRows);
	someTeamBRows.Sort(sortRows);
}
