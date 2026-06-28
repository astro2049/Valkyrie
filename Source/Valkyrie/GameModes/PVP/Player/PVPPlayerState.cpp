// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPPlayerState.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/GameModes/PVP/PVPGameState.h"

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
	NotifyGameStateChanged();
}

void APVPPlayerState::OnRep_TeamId() const
{
	NotifyGameStateChanged();
}

void APVPPlayerState::NotifyGameStateChanged() const
{
	if (const UWorld* const world = GetWorld()) {
		if (APVPGameState* const gameState = world->GetGameState<APVPGameState>()) {
			gameState->NotifyStateChanged();
		}
	}
}
