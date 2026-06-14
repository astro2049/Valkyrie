// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionHUDViewModel.h"

#include "Valkyrie/GameModes/Extraction/ExtractionGameState.h"

void UExtractionHUDViewModel::BindToExtractionGameState(AExtractionGameState* aGameState)
{
	if (myExtractionGameState == aGameState) {
		RefreshFromExtractionGameState();
		return;
	}

	UnbindExtractionGameState();

	myExtractionGameState = aGameState;
	if (myExtractionGameState) {
		myExtractionGameState->OnObjectiveChanged.AddUniqueDynamic(
			this,
			&UExtractionHUDViewModel::HandleObjectiveChanged
		);
		myExtractionGameState->OnDefenseTimerChanged.AddUniqueDynamic(
			this,
			&UExtractionHUDViewModel::HandleDefenseTimerChanged
		);
	}

	RefreshFromExtractionGameState();
}

void UExtractionHUDViewModel::UnbindExtractionGameState()
{
	if (myExtractionGameState) {
		myExtractionGameState->OnObjectiveChanged.RemoveDynamic(
			this,
			&UExtractionHUDViewModel::HandleObjectiveChanged
		);
		myExtractionGameState->OnDefenseTimerChanged.RemoveDynamic(
			this,
			&UExtractionHUDViewModel::HandleDefenseTimerChanged
		);
	}

	myExtractionGameState = nullptr;
	myObjectiveText = FText::GetEmpty();
	myDefenseTimeRemaining = 0.f;
	myShowDefenseTimer = false;
	BroadcastViewModelChanged();
}

void UExtractionHUDViewModel::RefreshFromExtractionGameState()
{
	if (!myExtractionGameState) {
		myObjectiveText = FText::GetEmpty();
		myDefenseTimeRemaining = 0.f;
		myShowDefenseTimer = false;
		BroadcastViewModelChanged();
		return;
	}

	myObjectiveText = myExtractionGameState->GetObjectiveText();
	myDefenseTimeRemaining = myExtractionGameState->GetDefenseTimeRemaining();
	myShowDefenseTimer = myExtractionGameState->ShouldShowDefenseTimer();
	BroadcastViewModelChanged();
}

void UExtractionHUDViewModel::HandleObjectiveChanged(FText anObjectiveText)
{
	myObjectiveText = anObjectiveText;
	BroadcastViewModelChanged();
}

void UExtractionHUDViewModel::HandleDefenseTimerChanged(float aDefenseTimeRemaining, bool aShowDefenseTimer)
{
	myDefenseTimeRemaining = aDefenseTimeRemaining;
	myShowDefenseTimer = aShowDefenseTimer;
	BroadcastViewModelChanged();
}
