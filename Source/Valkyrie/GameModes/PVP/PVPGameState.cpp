// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPGameState.h"

#include "Net/UnrealNetwork.h"

void APVPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APVPGameState, myMatchEnded);
	DOREPLIFETIME(APVPGameState, myWinningTeamId);
}

void APVPGameState::AddPlayerState(APlayerState* const aPlayerState)
{
	Super::AddPlayerState(aPlayerState);
	myOnPVPGameStateChanged.Broadcast();
}

void APVPGameState::RemovePlayerState(APlayerState* const aPlayerState)
{
	Super::RemovePlayerState(aPlayerState);
	myOnPVPGameStateChanged.Broadcast();
}

void APVPGameState::SetWinningTeamId(const int32 aTeamId)
{
	if (!HasAuthority() || myMatchEnded) {
		return;
	}

	myWinningTeamId = aTeamId;
	myMatchEnded = true;
	myOnPVPGameStateChanged.Broadcast();
}

void APVPGameState::OnRep_MatchResult()
{
	myOnPVPGameStateChanged.Broadcast();
}
