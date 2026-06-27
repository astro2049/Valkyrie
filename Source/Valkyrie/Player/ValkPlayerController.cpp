// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerController.h"

#include "Components/Widget.h"
#include "EnhancedInputSubsystems.h"

void AValkPlayerController::SetInputGameOnly(bool aShowMouseCursor)
{
	bShowMouseCursor = aShowMouseCursor;

	FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
}

void AValkPlayerController::SetInputUIOnly(UWidget* aWidgetToFocus, bool aShowMouseCursor)
{
	bShowMouseCursor = aShowMouseCursor;

	FInputModeUIOnly inputMode;
	if (aWidgetToFocus) {
		inputMode.SetWidgetToFocus(aWidgetToFocus->TakeWidget());
	}
	SetInputMode(inputMode);
}

void AValkPlayerController::AddInputMappingContext() const
{
	if (!IsLocalController() || !myInputMappingContext) {
		return;
	}

	ULocalPlayer* localPlayer = GetLocalPlayer();
	if (!localPlayer) {
		return;
	}
	
	if (UEnhancedInputLocalPlayerSubsystem* inputSubsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
		inputSubsystem->AddMappingContext(myInputMappingContext, 0);
	}
}

void AValkPlayerController::RestartCurrentLevel_Implementation()
{
	if (HasAuthority()) {
		ReturnAllPlayersToMainMenu();
	} else {
		Server_ReturnAllPlayersToMainMenu();
	}
}

void AValkPlayerController::ReturnAllPlayersToMainMenu()
{
	if (UWorld* world = GetWorld()) {
		world->ServerTravel(TEXT("/Game/MainMenu/MainMenu"));
	}
}

void AValkPlayerController::Server_ReturnAllPlayersToMainMenu_Implementation()
{
	ReturnAllPlayersToMainMenu();
}
