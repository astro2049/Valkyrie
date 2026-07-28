// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetTDMScoreboardData.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "Valkyrie/GameModes/TDM/TDMGameState.h"
#include "Valkyrie/Player/States/TDM/TDMPlayerState.h"

namespace
{
	bool IsBeforeOnTDMScoreboard(const ATDMPlayerState& aLeft, const ATDMPlayerState& aRight)
	{
		bool isBefore = false;
		if (aLeft.GetKills() != aRight.GetKills()) {
			isBefore = aLeft.GetKills() > aRight.GetKills();
		} else if (aLeft.GetDeaths() != aRight.GetDeaths()) {
			isBefore = aLeft.GetDeaths() < aRight.GetDeaths();
		} else {
			isBefore = aLeft.GetPlayerName() < aRight.GetPlayerName();
		}
		return isBefore;
	}

	const ATDMGameState* GetTDMGameState(const APlayerController* const aPlayerController)
	{
		const ATDMGameState* gameState = nullptr;
		if (aPlayerController) {
			if (const UWorld* const world = aPlayerController->GetWorld()) {
				gameState = world->GetGameState<ATDMGameState>();
			}
		}
		return gameState;
	}

	void FillScoreboardRows(
		const TArray<const ATDMPlayerState*>& somePlayerStates,
		TArray<FValkTDMScoreboardRowData>& someRows
	)
	{
		for (const ATDMPlayerState* const playerState : somePlayerStates) {
			if (playerState && someRows.Num() < ValkGameRules::MaxPlayersPerTeam) {
				FValkTDMScoreboardRowData row;
				row.myPlayerName = playerState->GetPlayerName();
				row.myKills = playerState->GetKills();
				row.myDeaths = playerState->GetDeaths();
				someRows.Add(row);
			}
		}
	}
}

void UUINode_GetTDMScoreboardData::GetTDMScoreboardData(
	APlayerController* const aPlayerController,
	int32& aTeamAScore,
	int32& aTeamBScore,
	TArray<FValkTDMScoreboardRowData>& someTeamARows,
	TArray<FValkTDMScoreboardRowData>& someTeamBRows
)
{
	aTeamAScore = 0;
	aTeamBScore = 0;
	someTeamARows.Reset();
	someTeamBRows.Reset();
	someTeamARows.Reserve(ValkGameRules::MaxPlayersPerTeam);
	someTeamBRows.Reserve(ValkGameRules::MaxPlayersPerTeam);

	if (const ATDMGameState* const gameState = GetTDMGameState(aPlayerController)) {
		aTeamAScore = gameState->GetTeamAKills();
		aTeamBScore = gameState->GetTeamBKills();

		TArray<const ATDMPlayerState*> teamAPlayerStates;
		TArray<const ATDMPlayerState*> teamBPlayerStates;
		for (const APlayerState* const playerState : gameState->PlayerArray) {
			if (const ATDMPlayerState* const tdmPlayerState = Cast<ATDMPlayerState>(playerState)) {
				if (tdmPlayerState->GetTeamId() == EValkTeamId::TeamA) {
					teamAPlayerStates.Add(tdmPlayerState);
				} else if (tdmPlayerState->GetTeamId() == EValkTeamId::TeamB) {
					teamBPlayerStates.Add(tdmPlayerState);
				}
			}
		}

		teamAPlayerStates.Sort(IsBeforeOnTDMScoreboard);
		teamBPlayerStates.Sort(IsBeforeOnTDMScoreboard);
		FillScoreboardRows(teamAPlayerStates, someTeamARows);
		FillScoreboardRows(teamBPlayerStates, someTeamBRows);
	}
}
