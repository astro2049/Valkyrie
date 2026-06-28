// Fill out your copyright notice in the Description page of Project Settings.

#include "UIPVPHUD.h"

#include "Components/TextBlock.h"
#include "PVPHUDViewModel.h"

void UUIPVPHUD::UpdateModeFromViewModel() const
{
	const UPVPHUDViewModel* const viewModel = Cast<UPVPHUDViewModel>(GetViewModel());
	if (!viewModel) {
		return;
	}

	const FPVPHUDData& pvpHUDData = viewModel->GetPVPHUDData();
	if (myTeamAScoreText) {
		myTeamAScoreText->SetText(pvpHUDData.myTeamAScoreText);
	}
	if (myTeamBScoreText) {
		myTeamBScoreText->SetText(pvpHUDData.myTeamBScoreText);
	}
	if (myWinnerText) {
		myWinnerText->SetText(pvpHUDData.myWinnerText);
		myWinnerText->SetVisibility(pvpHUDData.myShowWinner ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
