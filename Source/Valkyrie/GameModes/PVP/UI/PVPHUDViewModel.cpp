// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPHUDViewModel.h"

#include "Valkyrie/GameModes/PVP/PVPGameState.h"

void UPVPHUDViewModel::BindToGameState(APVPGameState* const aGameState)
{
	if (myPVPGameState.Get() != aGameState) {
		if (myPVPGameState.IsValid()) {
			myPVPGameState->myOnPVPGameStateChanged.RemoveAll(this);
		}
		myPVPGameState = aGameState;
		if (myPVPGameState.IsValid()) {
			myPVPGameState->myOnPVPGameStateChanged.AddUObject(
				this,
				&UPVPHUDViewModel::HandlePVPGameStateChanged
			);
		}
	}

	RefreshModeHUDData();
	BroadcastViewModelChanged();
}

void UPVPHUDViewModel::HandlePVPGameStateChanged()
{
	RefreshModeHUDData();
	BroadcastViewModelChanged();
}
