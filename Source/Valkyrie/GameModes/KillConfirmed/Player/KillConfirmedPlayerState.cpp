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
		NotifyGameStateChanged();
	}
}

void AKillConfirmedPlayerState::AddDeath()
{
	if (HasAuthority()) {
		++myDeaths;
		NotifyGameStateChanged();
	}
}

void AKillConfirmedPlayerState::AddConfirm()
{
	if (HasAuthority()) {
		++myConfirms;
		NotifyGameStateChanged();
	}
}

void AKillConfirmedPlayerState::OnRep_CombatStats()
{
	NotifyGameStateChanged();
}
