// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionGameState.h"

#include "Net/UnrealNetwork.h"

void AExtractionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExtractionGameState, myCombatSliceState);
	DOREPLIFETIME(AExtractionGameState, myDefenseTimeRemaining);
}

void AExtractionGameState::SetCombatSliceState(ECombatSliceState aCombatSliceState)
{
	myCombatSliceState = aCombatSliceState;
}

void AExtractionGameState::SetDefenseTimeRemaining(const float aDefenseTimeRemaining)
{
	myDefenseTimeRemaining = aDefenseTimeRemaining;
}
