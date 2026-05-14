// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatSliceGameState.h"

void ACombatSliceGameState::SetCombatSliceState(ECombatSliceState aCombatSliceState)
{
	myCombatSliceState = aCombatSliceState;
	OnCombatSliceStateChanged.Broadcast(myCombatSliceState);
}

void ACombatSliceGameState::SetObjectiveText(const FText& anObjectiveText)
{
	myObjectiveText = anObjectiveText;
	OnObjectiveChanged.Broadcast(myObjectiveText);
}

void ACombatSliceGameState::SetDefenseTimer(float aDefenseTimeRemaining, bool aShowDefenseTimer)
{
	myDefenseTimeRemaining = aDefenseTimeRemaining;
	myShowDefenseTimer = aShowDefenseTimer;
	OnDefenseTimerChanged.Broadcast(myDefenseTimeRemaining, myShowDefenseTimer);
}
