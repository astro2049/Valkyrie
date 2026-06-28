// Fill out your copyright notice in the Description page of Project Settings.

#include "KillConfirmedPlayerController.h"

#include "Valkyrie/GameModes/KillConfirmed/UI/KillConfirmedHUDViewModel.h"
#include "Valkyrie/GameModes/KillConfirmed/UI/KillConfirmedScoreboardViewModel.h"

UPVPHUDViewModel* AKillConfirmedPlayerController::CreateHUDViewModel()
{
	return NewObject<UKillConfirmedHUDViewModel>(this);
}

UPVPScoreboardViewModel* AKillConfirmedPlayerController::CreateScoreboardViewModel()
{
	return NewObject<UKillConfirmedScoreboardViewModel>(this);
}
