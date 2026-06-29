// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionGameState.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/UI/UIMessageSubsystem.h"

void AExtractionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExtractionGameState, myCombatSliceState);
	DOREPLIFETIME(AExtractionGameState, myDefenseTimeRemaining);
}

void AExtractionGameState::SetCombatSliceState(ECombatSliceState aCombatSliceState)
{
	myCombatSliceState = aCombatSliceState;
	BroadcastStateChanged();
}

void AExtractionGameState::SetDefenseTimeRemaining(const float aDefenseTimeRemaining)
{
	myDefenseTimeRemaining = aDefenseTimeRemaining;
	BroadcastStateChanged();
}

void AExtractionGameState::BroadcastStateChanged() const
{
	if (UUIMessageSubsystem* const messageSubsystem = VALK_UISUBSYS()) {
		messageSubsystem->BroadcastUIMessage(EUIMessageType::GameStateUpdated);
	}
}
