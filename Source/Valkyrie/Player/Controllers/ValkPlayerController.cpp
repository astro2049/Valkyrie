// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerController.h"

#include "Components/Widget.h"
#include "EnhancedInputSubsystems.h"

void AValkPlayerController::AddInputMappingContext() const
{
	if (IsLocalController() && myInputMappingContext) {
		if (const ULocalPlayer* const localPlayer = GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* const inputSubsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
				inputSubsystem->AddMappingContext(myInputMappingContext, 0);
			}
		}
	}
}

void AValkPlayerController::SetInputGameOnly()
{
	bShowMouseCursor = false;
	const FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
}

void AValkPlayerController::SetInputUIOnly(UWidget* const aWidgetToFocus)
{
	bShowMouseCursor = true;
	FInputModeUIOnly inputMode;
	if (aWidgetToFocus) {
		inputMode.SetWidgetToFocus(aWidgetToFocus->TakeWidget());
	}
	SetInputMode(inputMode);
}

