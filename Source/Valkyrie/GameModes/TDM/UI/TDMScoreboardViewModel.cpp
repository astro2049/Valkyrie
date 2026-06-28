// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMScoreboardViewModel.h"

#include "Valkyrie/GameModes/TDM/TDMGameState.h"
#include "Valkyrie/GameModes/TDM/Player/TDMPlayerState.h"

bool UTDMScoreboardViewModel::BuildRowData(
	const APlayerState* const aPlayerState,
	FPVPScoreboardRowData& aRowData
) const
{
	const ATDMPlayerState* const playerState = Cast<ATDMPlayerState>(aPlayerState);
	if (!playerState) {
		return false;
	}

	aRowData.myPlayerNameText = FText::FromString(playerState->GetPlayerName());
	aRowData.myKills = playerState->GetKills();
	aRowData.myDeaths = playerState->GetDeaths();
	aRowData.myKillsText = FText::AsNumber(aRowData.myKills);
	aRowData.myDeathsText = FText::AsNumber(aRowData.myDeaths);
	return true;
}

void UTDMScoreboardViewModel::BuildScoreText(
	FText& aTeamAScoreText,
	FText& aTeamBScoreText
) const
{
	const ATDMGameState* const gameState = Cast<ATDMGameState>(GetPVPGameState());
	aTeamAScoreText = FText::AsNumber(gameState ? gameState->GetTeamAKills() : 0);
	aTeamBScoreText = FText::AsNumber(gameState ? gameState->GetTeamBKills() : 0);
}
