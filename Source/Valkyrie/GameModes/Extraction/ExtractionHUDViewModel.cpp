// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionHUDViewModel.h"

#include "Valkyrie/GameModes/Extraction/ExtractionGameState.h"

void UExtractionHUDViewModel::BindToGameState(AExtractionGameState* aGameState)
{
	if (myExtractionGameState.Get() == aGameState) {
		RefreshFromExtractionGameState();
		return;
	}

	myExtractionGameState = aGameState;
	if (myExtractionGameState.IsValid()) {
		myExtractionGameState->myOnExtractionStateChanged.AddUObject(
			this,
			&UExtractionHUDViewModel::HandleExtractionStateChanged
		);
	}

	RefreshFromExtractionGameState();
}

void UExtractionHUDViewModel::RefreshFromExtractionGameState()
{
	if (myExtractionGameState.IsValid()) {
		myExtractionHUDData.myObjectiveText = myExtractionGameState->GetObjectiveText();
		myExtractionHUDData.myDefenseTimeRemaining = myExtractionGameState->GetDefenseTimeRemaining();
		myExtractionHUDData.myShowDefenseTimer = myExtractionGameState->ShouldShowDefenseTimer();
	} else {
		myExtractionHUDData = FExtractionHUDData{};
	}

	BroadcastViewModelChanged();
}

void UExtractionHUDViewModel::HandleExtractionStateChanged()
{
	RefreshFromExtractionGameState();
}
