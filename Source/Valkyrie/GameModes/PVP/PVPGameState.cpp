// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPGameState.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/UI/UIMessageSubsystem.h"

void APVPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APVPGameState, myMatchEnded);
	DOREPLIFETIME(APVPGameState, myWinningTeamId);
}

void APVPGameState::AddPlayerState(APlayerState* const aPlayerState)
{
	Super::AddPlayerState(aPlayerState);
	BroadcastStateChanged();
}

void APVPGameState::RemovePlayerState(APlayerState* const aPlayerState)
{
	Super::RemovePlayerState(aPlayerState);
	BroadcastStateChanged();
}

void APVPGameState::SetWinningTeamId(const int32 aTeamId)
{
	if (!HasAuthority() || myMatchEnded) {
		return;
	}

	myWinningTeamId = aTeamId;
	myMatchEnded = true;
	BroadcastStateChanged();
}

void APVPGameState::BroadcastStateChanged() const
{
	if (UUIMessageSubsystem* const messageSubsystem = VALK_UISUBSYS()) {
		messageSubsystem->BroadcastUIMessage(EUIMessageType::GameStateUpdated);
	}
}
