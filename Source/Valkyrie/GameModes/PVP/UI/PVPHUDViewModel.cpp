// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPHUDViewModel.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/GameModes/PVP/PVPGameState.h"

void UPVPHUDViewModel::RefreshData()
{
	RefreshModeHUDData();
}

APVPGameState* UPVPHUDViewModel::GetPVPGameState() const
{
	const APlayerController* const playerController = GetPlayerController();
	const UWorld* const world = playerController ? playerController->GetWorld() : nullptr;
	return world ? world->GetGameState<APVPGameState>() : nullptr;
}
