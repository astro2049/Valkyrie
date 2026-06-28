// Fill out your copyright notice in the Description page of Project Settings.

#include "UIPVPScoreboardRow.h"

#include "Components/TextBlock.h"
#include "PVPScoreboardViewModel.h"

void UUIPVPScoreboardRow::SetData(const FPVPScoreboardRowData& aRowData, const bool aShowConfirms) const
{
	if (myPlayerNameText) {
		myPlayerNameText->SetText(aRowData.myPlayerNameText);
	}
	if (myConfirmsText) {
		myConfirmsText->SetText(aRowData.myConfirmsText);
		myConfirmsText->SetVisibility(aShowConfirms ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	if (myKillsText) {
		myKillsText->SetText(aRowData.myKillsText);
	}
	if (myDeathsText) {
		myDeathsText->SetText(aRowData.myDeathsText);
	}
}
