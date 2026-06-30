// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPGameState.h"

#include "Net/UnrealNetwork.h"

void APVPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APVPGameState, myMatchEnded);
	DOREPLIFETIME(APVPGameState, myWinningTeamId);
}

void APVPGameState::SetWinningTeamId(const int32 aTeamId)
{
	if (!HasAuthority() || myMatchEnded) {
		return;
	}

	myWinningTeamId = aTeamId;
	myMatchEnded = true;
}
