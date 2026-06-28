// Fill out your copyright notice in the Description page of Project Settings.

#include "UIExtractionHUD.h"

#include "Components/TextBlock.h"
#include "ExtractionHUDViewModel.h"

void UUIExtractionHUD::UpdateModeFromViewModel() const
{
	const UExtractionHUDViewModel* const viewModel = Cast<UExtractionHUDViewModel>(GetViewModel());
	if (!viewModel) {
		return;
	}

	const FExtractionHUDData& extractionHUDData = viewModel->GetExtractionHUDData();
	if (myObjectiveText) {
		myObjectiveText->SetText(extractionHUDData.myObjectiveText);
	}
	if (myDefenseTimerText) {
		const bool showDefenseTimer = extractionHUDData.myShowDefenseTimer;
		myDefenseTimerText->SetVisibility(showDefenseTimer ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		myDefenseTimerText->SetText(showDefenseTimer
			? FText::AsNumber(FMath::CeilToInt(extractionHUDData.myDefenseTimeRemaining))
			: FText::GetEmpty());
	}
}
