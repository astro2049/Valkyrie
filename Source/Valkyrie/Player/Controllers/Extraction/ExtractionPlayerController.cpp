// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionPlayerController.h"

#include "Blueprint/UserWidget.h"

void AExtractionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) {
		return;
	}

	if (myHUDWidgetClass) {
		myHUDWidget = CreateWidget<UUserWidget>(this, myHUDWidgetClass);
		if (myHUDWidget) {
			myHUDWidget->AddToViewport();
		}
	}
}

void AExtractionPlayerController::OnPlayerDeath()
{
	if (!IsLocalController()) {
		return;
	}

	ShowDeadOverlay();
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
	if (APawn* const controlledPawn = GetPawn()) {
		controlledPawn->DisableInput(this);
	}
}

void AExtractionPlayerController::ShowDeadOverlay()
{
	if (!IsLocalController() || myDeadOverlayWidget || !myDeadOverlayWidgetClass) {
		return;
	}

	myDeadOverlayWidget = CreateWidget<UUserWidget>(this, myDeadOverlayWidgetClass);
	if (myDeadOverlayWidget) {
		myDeadOverlayWidget->AddToViewport();
	}
}

