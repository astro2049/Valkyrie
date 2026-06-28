// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionPlayerController.h"

#include "Valkyrie/GameModes/Extraction/UI/UIExtractionHUD.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameState.h"
#include "Blueprint/UserWidget.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"
#include "Valkyrie/GameModes/Extraction/UI/ExtractionHUDViewModel.h"

void AExtractionPlayerController::CreateModeUI()
{
	if (!myHUDViewModel) {
		myHUDViewModel = NewObject<UExtractionHUDViewModel>(this);
	}

	if (!myHUDWidget && myHUDWidgetClass) {
		myHUDWidget = CreateWidget<UUIExtractionHUD>(this, myHUDWidgetClass);
		if (myHUDWidget) {
			myHUDWidget->SetViewModel(myHUDViewModel);
			myHUDWidget->AddToViewport();
		}
	}
}

void AExtractionPlayerController::InitializeModeState()
{
	if (!myHUDViewModel || !GetWorld()) {
		return;
	}

	AExtractionGameState* const extractionGameState = GetWorld()->GetGameState<AExtractionGameState>();
	myHUDViewModel->BindToGameState(extractionGameState);

	if (myBoundExtractionGameState.Get() != extractionGameState) {
		myBoundExtractionGameState = extractionGameState;
		if (myBoundExtractionGameState.IsValid()) {
			myBoundExtractionGameState->myOnExtractionStateChanged.AddUObject(
				this,
				&AExtractionPlayerController::HandleExtractionStateChanged
			);
			HandleExtractionStateChanged();
		}
	}
}

void AExtractionPlayerController::SetModePawn(AValkPlayerPawn* const aPlayerPawn)
{
	if (myHUDViewModel) {
		myHUDViewModel->BindToPawn(aPlayerPawn);
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

void AExtractionPlayerController::HandleExtractionStateChanged()
{
	if (!myBoundExtractionGameState.IsValid()) {
		return;
	}

	const ECombatSliceState combatSliceState = myBoundExtractionGameState->GetCombatSliceState();
	if (combatSliceState == ECombatSliceState::Completed) {
		ShowVictoryMenu();
	} else if (combatSliceState == ECombatSliceState::Failed) {
		ShowFailureMenu();
	}
}
