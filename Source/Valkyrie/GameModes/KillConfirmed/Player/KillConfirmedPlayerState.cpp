// Fill out your copyright notice in the Description page of Project Settings.

#include "KillConfirmedPlayerState.h"

#include "Net/UnrealNetwork.h"

void AKillConfirmedPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKillConfirmedPlayerState, myKills);
	DOREPLIFETIME(AKillConfirmedPlayerState, myDeaths);
	DOREPLIFETIME(AKillConfirmedPlayerState, myConfirms);
}

void AKillConfirmedPlayerState::AddKill()
{
	if (HasAuthority()) {
		++myKills;
		myOnPlayerStateChanged.Broadcast();
	}
}

void AKillConfirmedPlayerState::AddDeath()
{
	if (HasAuthority()) {
		++myDeaths;
		myOnPlayerStateChanged.Broadcast();
	}
}

void AKillConfirmedPlayerState::AddConfirm()
{
	if (HasAuthority()) {
		++myConfirms;
		myOnPlayerStateChanged.Broadcast();
	}
}

void AKillConfirmedPlayerState::OnRep_CombatStats()
{
	myOnPlayerStateChanged.Broadcast();
}
