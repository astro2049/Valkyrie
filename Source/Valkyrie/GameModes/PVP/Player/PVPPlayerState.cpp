// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPPlayerState.h"

#include "Net/UnrealNetwork.h"

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
	myOnPlayerStateChanged.Broadcast();
}

void APVPPlayerState::OnRep_TeamId() const
{
	myOnPlayerStateChanged.Broadcast();
}
