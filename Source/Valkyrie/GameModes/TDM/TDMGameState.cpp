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
	check(aTeamId == EValkTeamId::TeamA || aTeamId == EValkTeamId::TeamB);
	int32 teamKills;
	if (aTeamId == EValkTeamId::TeamA) {
		teamKills = ++myTeamAKills;
	} else {
		teamKills = ++myTeamBKills;
	}
	return teamKills;
}
