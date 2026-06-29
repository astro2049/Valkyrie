// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionGameState.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/UI/UIMessageSubsystem.h"

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
	NotifyStateChanged();
}

void AExtractionGameState::SetObjectiveText(const FText& anObjectiveText)
{
	myObjectiveText = anObjectiveText;
	NotifyStateChanged();
}

void AExtractionGameState::SetDefenseTimer(float aDefenseTimeRemaining, bool aShowDefenseTimer)
{
	myDefenseTimeRemaining = aDefenseTimeRemaining;
	myShowDefenseTimer = aShowDefenseTimer;
	NotifyStateChanged();
}

void AExtractionGameState::NotifyStateChanged() const
{
	myOnExtractionStateChanged.Broadcast();
	if (UUIMessageSubsystem* const messageSubsystem = VALK_UISUBSYS()) {
		messageSubsystem->BroadcastUIMessage(EUIMessageType::GameStateUpdated);
	}
}

void AExtractionGameState::OnRep_CombatSliceState()
{
	NotifyStateChanged();
}

void AExtractionGameState::OnRep_ObjectiveText()
{
	NotifyStateChanged();
}

void AExtractionGameState::OnRep_DefenseTimer()
{
	NotifyStateChanged();
}
