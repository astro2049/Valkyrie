// Fill out your copyright notice in the Description page of Project Settings.

#include "KillConfirmedScoreboardViewModel.h"

#include "Valkyrie/GameModes/KillConfirmed/KillConfirmedGameState.h"
#include "Valkyrie/GameModes/KillConfirmed/Player/KillConfirmedPlayerState.h"

bool UKillConfirmedScoreboardViewModel::BuildRowData(
	const APlayerState* const aPlayerState,
	FPVPScoreboardRowData& aRowData
) const
{
	const AKillConfirmedPlayerState* const playerState = Cast<AKillConfirmedPlayerState>(aPlayerState);
	if (!playerState) {
		return false;
	}

	aRowData.myPlayerNameText = FText::FromString(playerState->GetPlayerName());
	aRowData.myKills = playerState->GetKills();
	aRowData.myDeaths = playerState->GetDeaths();
	aRowData.myConfirmsText = FText::AsNumber(playerState->GetConfirms());
	aRowData.myKillsText = FText::AsNumber(aRowData.myKills);
	aRowData.myDeathsText = FText::AsNumber(aRowData.myDeaths);
	return true;
}

void UKillConfirmedScoreboardViewModel::BuildScoreText(
	FText& aTeamAScoreText,
	FText& aTeamBScoreText
) const
{
	const AKillConfirmedGameState* const gameState = Cast<AKillConfirmedGameState>(GetPVPGameState());
	aTeamAScoreText = FText::AsNumber(gameState ? gameState->GetTeamAConfirms() : 0);
	aTeamBScoreText = FText::AsNumber(gameState ? gameState->GetTeamBConfirms() : 0);
}
