// Fill out your copyright notice in the Description page of Project Settings.

#include "KillConfirmedGameState.h"

#include "Net/UnrealNetwork.h"
#include "Valkyrie/Common/ValkTypes.h"

void AKillConfirmedGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKillConfirmedGameState, myTeamAConfirms);
	DOREPLIFETIME(AKillConfirmedGameState, myTeamBConfirms);
}

int32 AKillConfirmedGameState::AddTeamConfirm(const EValkTeamId aTeamId)
{
	if (!HasAuthority()) {
		return 0;
	}

	int32 teamConfirms = 0;
	if (aTeamId == EValkTeamId::TeamA) {
		teamConfirms = ++myTeamAConfirms;
	} else if (aTeamId == EValkTeamId::TeamB) {
		teamConfirms = ++myTeamBConfirms;
	}
	return teamConfirms;
}
