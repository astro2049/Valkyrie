// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPScoreboardViewModel.h"

#include "Valkyrie/GameModes/PVP/PVPGameState.h"
#include "Valkyrie/GameModes/PVP/Player/PVPPlayerState.h"
#include "Valkyrie/GameModes/PVP/PVPTypes.h"

void UPVPScoreboardViewModel::BindToGameState(APVPGameState* const aGameState)
{
	if (myPVPGameState.Get() != aGameState) {
		if (myPVPGameState.IsValid()) {
			myPVPGameState->myOnPVPGameStateChanged.RemoveAll(this);
		}
		myPVPGameState = aGameState;
		if (myPVPGameState.IsValid()) {
			myPVPGameState->myOnPVPGameStateChanged.AddUObject(
				this,
				&UPVPScoreboardViewModel::HandleGameStateChanged
			);
		}
	}

	RebuildScoreboardData();
	BroadcastViewModelChanged();
}

void UPVPScoreboardViewModel::HandleGameStateChanged()
{
	RebuildScoreboardData();
	BroadcastViewModelChanged();
}

void UPVPScoreboardViewModel::RebuildScoreboardData()
{
	myScoreboardData = {};
	BuildScoreText(myScoreboardData.myTeamAScoreText, myScoreboardData.myTeamBScoreText);
	if (!myPVPGameState.IsValid()) {
		return;
	}

	for (const APlayerState* const playerState : myPVPGameState->PlayerArray) {
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
