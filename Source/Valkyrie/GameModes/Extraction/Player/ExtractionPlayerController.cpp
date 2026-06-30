// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameState.h"
#include "Valkyrie/GameModes/Extraction/UI/UIExtractionHUD.h"
#include "Valkyrie/UI/UIValkHUD.h"

void AExtractionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) {
		return;
	}

	if (!myPlayerHUDWidget && myPlayerHUDWidgetClass) {
		myPlayerHUDWidget = CreateWidget<UUIValkHUD>(this, myPlayerHUDWidgetClass);
		if (myPlayerHUDWidget) {
			myPlayerHUDWidget->AddToViewport();
		}
	}
	if (!myModeWidget && myModeWidgetClass) {
		myModeWidget = CreateWidget<UUIExtractionHUD>(this, myModeWidgetClass);
		if (myModeWidget) {
			myModeWidget->AddToViewport();
		}
	}
}

void AExtractionPlayerController::Tick(const float aDeltaSeconds)
{
	Super::Tick(aDeltaSeconds);

	if (IsLocalController()) {
		UpdateEndMenuFromState();
	}
}

void AExtractionPlayerController::HandleLocalPlayerDeath()
{
	ShowDeadOverlay();
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
	if (APawn* const controlledPawn = GetPawn()) {
		controlledPawn->DisableInput(this);
	}
}

void AExtractionPlayerController::UpdateEndMenuFromState()
{
	const AExtractionGameState* const gameState = GetWorld()
		? GetWorld()->GetGameState<AExtractionGameState>()
		: nullptr;
	if (!gameState) {
		return;
	}

	const ECombatSliceState combatSliceState = gameState->GetCombatSliceState();
	if (combatSliceState == ECombatSliceState::Completed) {
		ShowVictoryMenu();
	} else if (combatSliceState == ECombatSliceState::Failed) {
		ShowFailureMenu();
	}
}

void AExtractionPlayerController::ShowDeadOverlay()
{
	if (IsLocalController() && !myDeadOverlayWidget && myDeadOverlayWidgetClass) {
		myDeadOverlayWidget = CreateWidget<UUserWidget>(this, myDeadOverlayWidgetClass);
		if (myDeadOverlayWidget) {
			myDeadOverlayWidget->AddToViewport();
		}
	}
}

void AExtractionPlayerController::ShowFailureMenu()
{
	if (IsLocalController() && !myDeathMenuWidget && myDeathMenuWidgetClass) {
		myDeathMenuWidget = CreateWidget<UUserWidget>(this, myDeathMenuWidgetClass);
		if (myDeathMenuWidget) {
			myDeathMenuWidget->AddToViewport();
			SetInputUIOnly(myDeathMenuWidget);
		}
	}
}

void AExtractionPlayerController::ShowVictoryMenu()
{
	if (!IsLocalController() || myVictoryMenuWidget || !myVictoryMenuWidgetClass) {
		return;
	}

	myVictoryMenuWidget = CreateWidget<UUserWidget>(this, myVictoryMenuWidgetClass);
	if (myVictoryMenuWidget) {
		myVictoryMenuWidget->AddToViewport();
		SetInputUIOnly(myVictoryMenuWidget);
	}
}
