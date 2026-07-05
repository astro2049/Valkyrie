// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetPVPScoreboardData.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/GameModes/PVP/KillConfirmed/KillConfirmedGameState.h"
#include "Valkyrie/GameModes/ValkGameState.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "Valkyrie/GameModes/PVP/TDM/TDMGameState.h"
#include "Valkyrie/Player/States/PVP/KillConfirmed/KillConfirmedPlayerState.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"
#include "Valkyrie/Player/States/PVP/TDM/TDMPlayerState.h"

void UUINode_GetPVPScoreboardData::GetPVPScoreboardData(
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

	if (aPlayerController) {
		if (const UWorld* const world = aPlayerController->GetWorld()) {
			if (const AValkGameState* const gameState = world->GetGameState<AValkGameState>()) {
				if (const AKillConfirmedGameState* const killConfirmedGameState = Cast<AKillConfirmedGameState>(gameState)) {
					aTeamAScore = killConfirmedGameState->GetTeamAConfirms();
					aTeamBScore = killConfirmedGameState->GetTeamBConfirms();
					aShowConfirms = true;
				} else if (const ATDMGameState* const tdmGameState = Cast<ATDMGameState>(gameState)) {
					aTeamAScore = tdmGameState->GetTeamAKills();
					aTeamBScore = tdmGameState->GetTeamBKills();
				}

				for (const APlayerState* const playerState : gameState->PlayerArray) {
					const AValkPlayerState* const pvpPlayerState = Cast<AValkPlayerState>(playerState);
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

					if (pvpPlayerState->GetTeamId() == EValkTeamId::TeamA) {
						someTeamARows.Add(MoveTemp(rowData));
					} else if (pvpPlayerState->GetTeamId() == EValkTeamId::TeamB) {
						someTeamBRows.Add(MoveTemp(rowData));
					}
				}

				auto sortRows = [](const FValkScoreboardRowData& aLeft, const FValkScoreboardRowData& aRight) {
					bool isLeftBeforeRight = false;
					if (aLeft.myKills != aRight.myKills) {
						isLeftBeforeRight = aLeft.myKills > aRight.myKills;
					} else if (aLeft.myDeaths != aRight.myDeaths) {
						isLeftBeforeRight = aLeft.myDeaths < aRight.myDeaths;
					} else {
						isLeftBeforeRight = aLeft.myPlayerName < aRight.myPlayerName;
					}
					return isLeftBeforeRight;
				};
				someTeamARows.Sort(sortRows);
				someTeamBRows.Sort(sortRows);
			}
		}
	}
}
