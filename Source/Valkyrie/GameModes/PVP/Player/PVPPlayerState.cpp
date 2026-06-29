// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPPlayerState.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/UI/UIMessageSubsystem.h"

void APVPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APVPPlayerState, myTeamId);
}

void APVPPlayerState::SetTeamId(const int32 aTeamId)
{
	if (myTeamId == aTeamId || !HasAuthority()) {
		return;
	}

	myTeamId = aTeamId;
	NotifyStateChanged();
}

void APVPPlayerState::OnRep_TeamId() const
{
	NotifyStateChanged();
}

void APVPPlayerState::NotifyStateChanged() const
{
	if (UUIMessageSubsystem* const messageSubsystem = VALK_UISUBSYS()) {
		messageSubsystem->BroadcastUIMessage(EUIMessageType::PlayerStateUpdated);
	}
}
