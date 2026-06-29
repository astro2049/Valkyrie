// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMPlayerState.h"

#include "Net/UnrealNetwork.h"

void ATDMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATDMPlayerState, myKills);
	DOREPLIFETIME(ATDMPlayerState, myDeaths);
}

void ATDMPlayerState::AddKill()
{
	if (HasAuthority()) {
		++myKills;
		NotifyStateChanged();
	}
}

void ATDMPlayerState::AddDeath()
{
	if (HasAuthority()) {
		++myDeaths;
		NotifyStateChanged();
	}
}

void ATDMPlayerState::OnRep_CombatStats()
{
	NotifyStateChanged();
}
