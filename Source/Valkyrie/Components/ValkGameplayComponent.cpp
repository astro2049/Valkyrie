// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkGameplayComponent.h"

#include "GameFramework/Pawn.h"
#include "Valkyrie/UI/UIMessageSubsystem.h"

void UValkGameplayComponent::BroadcastLocalPlayerUIMessage(const EUIMessageType aMessageType) const
{
	const APawn* const ownerPawn = Cast<APawn>(GetOwner());
	if (!ownerPawn || !ownerPawn->IsPlayerControlled() || !ownerPawn->IsLocallyControlled()) {
		return;
	}

	if (UUIMessageSubsystem* const messageSubsystem = VALK_UISUBSYS()) {
		messageSubsystem->BroadcastUIMessage(aMessageType);
	}
}
