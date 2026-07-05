// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMGameState.h"

#include "Net/UnrealNetwork.h"
#include "Valkyrie/Common/ValkTypes.h"

void ATDMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDMGameState, myTeamAKills);
	DOREPLIFETIME(ATDMGameState, myTeamBKills);
}

int32 ATDMGameState::AddTeamKill(const EValkTeamId aTeamId)
{
	if (!HasAuthority()) {
		return 0;
	}

	int32 teamKills = 0;
	if (aTeamId == EValkTeamId::TeamA) {
		teamKills = ++myTeamAKills;
	} else if (aTeamId == EValkTeamId::TeamB) {
		teamKills = ++myTeamBKills;
	}
	return teamKills;
}
