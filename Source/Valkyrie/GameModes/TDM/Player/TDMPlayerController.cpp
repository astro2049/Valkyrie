// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMPlayerController.h"

#include "Valkyrie/GameModes/TDM/UI/TDMHUDViewModel.h"
#include "Valkyrie/GameModes/TDM/UI/TDMScoreboardViewModel.h"

UPVPHUDViewModel* ATDMPlayerController::CreateHUDViewModel()
{
	return NewObject<UTDMHUDViewModel>(this);
}

UPVPScoreboardViewModel* ATDMPlayerController::CreateScoreboardViewModel()
{
	return NewObject<UTDMScoreboardViewModel>(this);
}
