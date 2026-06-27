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
	myOnExtractionStateChanged.Broadcast();
}

void AExtractionGameState::SetObjectiveText(const FText& anObjectiveText)
{
	myObjectiveText = anObjectiveText;
	myOnExtractionStateChanged.Broadcast();
}

void AExtractionGameState::SetDefenseTimer(float aDefenseTimeRemaining, bool aShowDefenseTimer)
{
	myDefenseTimeRemaining = aDefenseTimeRemaining;
	myShowDefenseTimer = aShowDefenseTimer;
	myOnExtractionStateChanged.Broadcast();
}

void AExtractionGameState::OnRep_CombatSliceState()
{
	myOnExtractionStateChanged.Broadcast();
}

void AExtractionGameState::OnRep_ObjectiveText()
{
	myOnExtractionStateChanged.Broadcast();
}

void AExtractionGameState::OnRep_DefenseTimer()
{
	myOnExtractionStateChanged.Broadcast();
}
