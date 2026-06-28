// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMHUDViewModel.h"

#include "Valkyrie/GameModes/TDM/TDMGameState.h"
#include "Valkyrie/GameModes/PVP/PVPTypes.h"

void UTDMHUDViewModel::RefreshModeHUDData()
{
	FPVPHUDData& hudData = GetMutablePVPHUDData();
	hudData = {};
	const ATDMGameState* const gameState = Cast<ATDMGameState>(GetPVPGameState());
	if (!gameState) {
		return;
	}

	hudData.myTeamAScoreText = FText::AsNumber(gameState->GetTeamAKills());
	hudData.myTeamBScoreText = FText::AsNumber(gameState->GetTeamBKills());
	hudData.myShowWinner = gameState->HasMatchEnded();
	if (hudData.myShowWinner) {
		hudData.myWinnerText = gameState->GetWinningTeamId() == ValkTeamId::TeamA
			? NSLOCTEXT("TDMHUD", "TeamAWins", "Team A Wins")
			: NSLOCTEXT("TDMHUD", "TeamBWins", "Team B Wins");
	}
}
