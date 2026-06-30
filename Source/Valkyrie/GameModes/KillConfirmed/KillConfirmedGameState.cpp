// Fill out your copyright notice in the Description page of Project Settings.

#include "KillConfirmedGameState.h"

#include "Net/UnrealNetwork.h"
#include "Valkyrie/GameModes/PVP/PVPTypes.h"

void AKillConfirmedGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKillConfirmedGameState, myTeamAConfirms);
	DOREPLIFETIME(AKillConfirmedGameState, myTeamBConfirms);
}

int32 AKillConfirmedGameState::AddTeamConfirm(const int32 aTeamId)
{
	if (!HasAuthority()) {
		return 0;
	}

	int32 teamConfirms = 0;
	if (aTeamId == ValkTeamId::TeamA) {
		teamConfirms = ++myTeamAConfirms;
	} else if (aTeamId == ValkTeamId::TeamB) {
		teamConfirms = ++myTeamBConfirms;
	}
	return teamConfirms;
}
