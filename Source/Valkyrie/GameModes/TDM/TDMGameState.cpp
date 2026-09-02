// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMGameState.h"

#include "Net/UnrealNetwork.h"
#include "Valkyrie/Common/ValkTypes.h"

void ATDMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDMGameState, myTeamAKills);
	DOREPLIFETIME(ATDMGameState, myTeamBKills);
	DOREPLIFETIME(ATDMGameState, myMatchHasEnded);
	DOREPLIFETIME(ATDMGameState, myWinningTeamId);
}

int32 ATDMGameState::AddTeamKill(const EValkTeamId aTeamId)
{
	check(aTeamId == EValkTeamId::TeamA || aTeamId == EValkTeamId::TeamB);
	int32 teamKills{-1};
	if (aTeamId == EValkTeamId::TeamA) {
		teamKills = ++myTeamAKills;
	} else if (aTeamId == EValkTeamId::TeamB) {
		teamKills = ++myTeamBKills;
	}
	return teamKills;
}

void ATDMGameState::SetMatchEnded(const EValkTeamId aTeamId)
{
	if (!myMatchHasEnded) {
		myWinningTeamId = aTeamId;
		myMatchHasEnded = true;
	}
}
