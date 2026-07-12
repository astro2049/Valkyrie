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
}

void UUINode_GetTDMScoreboardData::GetTDMScoreboardData(
	APlayerController* const aPlayerController,
	int32& aTeamAScore,
	int32& aTeamBScore,
	TArray<int32>& someTeamAPlayerIds,
	TArray<int32>& someTeamBPlayerIds
)
{
	aTeamAScore = 0;
	aTeamBScore = 0;
	someTeamAPlayerIds.Reset();
	someTeamBPlayerIds.Reset();

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
		for (const ATDMPlayerState* const playerState : teamAPlayerStates) {
			if (playerState) {
				someTeamAPlayerIds.Add(playerState->GetPlayerId());
			}
		}
		for (const ATDMPlayerState* const playerState : teamBPlayerStates) {
			if (playerState) {
				someTeamBPlayerIds.Add(playerState->GetPlayerId());
			}
		}
	}
}

void UUINode_GetTDMScoreboardData::GetTDMScoreboardRowData(
	APlayerController* const aPlayerController,
	const int32 aPlayerId,
	FString& aPlayerName,
	int32& aKills,
	int32& aDeaths
)
{
	aPlayerName = FString();
	aKills = -1;
	aDeaths = -1;

	if (const ATDMGameState* const gameState = GetTDMGameState(aPlayerController)) {
		for (const APlayerState* const playerState : gameState->PlayerArray) {
			if (const ATDMPlayerState* const tdmPlayerState = Cast<ATDMPlayerState>(playerState)) {
				if (tdmPlayerState->GetPlayerId() == aPlayerId) {
					aPlayerName = tdmPlayerState->GetPlayerName();
					aKills = tdmPlayerState->GetKills();
					aDeaths = tdmPlayerState->GetDeaths();
					break;
				}
			}
		}
	}
}
