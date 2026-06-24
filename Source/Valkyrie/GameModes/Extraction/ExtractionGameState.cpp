// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionGameState.h"

#include "Net/UnrealNetwork.h"

void AExtractionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExtractionGameState, myCombatSliceState);
	DOREPLIFETIME(AExtractionGameState, myObjectiveText);
	DOREPLIFETIME(AExtractionGameState, myDefenseTimeRemaining);
	DOREPLIFETIME(AExtractionGameState, myShowDefenseTimer);
}

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

void AExtractionGameState::OnRep_CombatSliceState()
{
	OnCombatSliceStateChanged.Broadcast(myCombatSliceState);
}

void AExtractionGameState::OnRep_ObjectiveText()
{
	OnObjectiveChanged.Broadcast(myObjectiveText);
}

void AExtractionGameState::OnRep_DefenseTimer()
{
	OnDefenseTimerChanged.Broadcast(myDefenseTimeRemaining, myShowDefenseTimer);
}
