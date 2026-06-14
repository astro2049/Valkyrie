// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionGameState.h"

void AExtractionGameState::SetCombatSliceState(ECombatSliceState aCombatSliceState)
{
	myCombatSliceState = aCombatSliceState;
	OnCombatSliceStateChanged.Broadcast(myCombatSliceState);
}

void AExtractionGameState::SetObjectiveText(const FText& anObjectiveText)
{
	myObjectiveText = anObjectiveText;
	OnObjectiveChanged.Broadcast(myObjectiveText);
}

void AExtractionGameState::SetDefenseTimer(float aDefenseTimeRemaining, bool aShowDefenseTimer)
{
	myDefenseTimeRemaining = aDefenseTimeRemaining;
	myShowDefenseTimer = aShowDefenseTimer;
	OnDefenseTimerChanged.Broadcast(myDefenseTimeRemaining, myShowDefenseTimer);
}
