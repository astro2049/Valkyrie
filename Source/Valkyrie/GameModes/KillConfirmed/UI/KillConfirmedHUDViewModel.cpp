// Fill out your copyright notice in the Description page of Project Settings.

#include "KillConfirmedHUDViewModel.h"

#include "Valkyrie/GameModes/KillConfirmed/KillConfirmedGameState.h"
#include "Valkyrie/GameModes/PVP/PVPTypes.h"

void UKillConfirmedHUDViewModel::RefreshModeHUDData()
{
	FPVPHUDData& hudData = GetMutablePVPHUDData();
	hudData = {};
	const AKillConfirmedGameState* const gameState = Cast<AKillConfirmedGameState>(GetPVPGameState());
	if (!gameState) {
		return;
	}

	hudData.myTeamAScoreText = FText::AsNumber(gameState->GetTeamAConfirms());
	hudData.myTeamBScoreText = FText::AsNumber(gameState->GetTeamBConfirms());
	hudData.myShowWinner = gameState->HasMatchEnded();
	if (hudData.myShowWinner) {
		hudData.myWinnerText = gameState->GetWinningTeamId() == ValkTeamId::TeamA
			? NSLOCTEXT("KCHUD", "TeamAWins", "Team A Wins")
			: NSLOCTEXT("KCHUD", "TeamBWins", "Team B Wins");
	}
}
