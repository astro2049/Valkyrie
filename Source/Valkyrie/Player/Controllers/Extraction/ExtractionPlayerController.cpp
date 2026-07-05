// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionPlayerController.h"

#include "Blueprint/UserWidget.h"

void AExtractionPlayerController::OnPlayerDeath()
{
	if (!IsLocalController()) {
		return;
	}

	Super::OnPlayerDeath();
	if (!myDeadOverlayWidget && myDeadOverlayWidgetClass) {
		myDeadOverlayWidget = CreateWidget<UUserWidget>(this, myDeadOverlayWidgetClass);
		if (myDeadOverlayWidget) {
			myDeadOverlayWidget->AddToViewport();
		}
	}
	SetIgnoreLookInput(true);
	if (APawn* const controlledPawn = GetPawn()) {
		controlledPawn->DisableInput(this);
	}
}

