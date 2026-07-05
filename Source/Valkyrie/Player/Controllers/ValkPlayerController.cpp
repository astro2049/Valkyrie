// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerController.h"

#include "Components/Widget.h"
#include "EnhancedInputSubsystems.h"

void AValkPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputGameOnly();
	AddInputMappingContext();
}

void AValkPlayerController::Client_OnPlayerDeath_Implementation()
{
	OnPlayerDeath();
}

void AValkPlayerController::OnPlayerDeath()
{
}

void AValkPlayerController::AddInputMappingContext() const
{
	if (!IsLocalController() || !myInputMappingContext) {
		return;
	}

	if (const ULocalPlayer* const localPlayer = GetLocalPlayer()) {
		if (UEnhancedInputLocalPlayerSubsystem* const inputSubsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
			inputSubsystem->AddMappingContext(myInputMappingContext, 0);
		}
	}
}

void AValkPlayerController::SetInputGameOnly()
{
	if (!IsLocalController()) {
		return;
	}

	bShowMouseCursor = false;
	const FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
}

void AValkPlayerController::SetInputUIOnly(UWidget* const aWidgetToFocus)
{
	if (!IsLocalController()) {
		return;
	}

	bShowMouseCursor = true;
	FInputModeUIOnly inputMode;
	if (aWidgetToFocus) {
		inputMode.SetWidgetToFocus(aWidgetToFocus->TakeWidget());
	}
	SetInputMode(inputMode);
}

