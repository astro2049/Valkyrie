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

int32 ATDMGameState::AddTeamKill(const int32 aTeamId)
{
	if (!HasAuthority()) {
		return 0;
	}

	int32 teamKills = 0;
	if (aTeamId == ValkTeamId::TeamA) {
		teamKills = ++myTeamAKills;
	} else if (aTeamId == ValkTeamId::TeamB) {
		teamKills = ++myTeamBKills;
	}
	return teamKills;
}
