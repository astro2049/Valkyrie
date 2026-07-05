// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkGameState.h"

#include "Net/UnrealNetwork.h"

void AValkGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AValkGameState, myMatchEnded);
	DOREPLIFETIME(AValkGameState, myWinningTeamId);
}

void AValkGameState::SetWinningTeamId(const EValkTeamId aTeamId)
{
	if (!HasAuthority() || myMatchEnded) {
		return;
	}

	myWinningTeamId = aTeamId;
	myMatchEnded = true;
}
