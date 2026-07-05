// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

void APVPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) {
		return;
	}

	OnPossessedPawnChanged.AddUniqueDynamic(
		this,
		&APVPPlayerController::OnPawnChanged
	);

	// UI widgets
	// HUD
	if (myHUDWidgetClass) {
		myHUDWidget = CreateWidget<UUserWidget>(this, myHUDWidgetClass);
		if (myHUDWidget) {
			myHUDWidget->AddToViewport();
		}
	}
	// Scoreboard
	if (myScoreboardWidgetClass) {
		myScoreboardWidget = CreateWidget<UUserWidget>(this, myScoreboardWidgetClass);
		if (myScoreboardWidget) {
			myScoreboardWidget->AddToViewport();
			myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APVPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!IsLocalController()) {
		return;
	}

	if (UEnhancedInputComponent* const enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		if (myInputActionOpenScoreboard) {
			enhancedInputComponent->BindAction(
				myInputActionOpenScoreboard,
				ETriggerEvent::Started,
				this,
				&APVPPlayerController::ShowScoreboard
			);
			enhancedInputComponent->BindAction(
				myInputActionOpenScoreboard,
				ETriggerEvent::Completed,
				this,
				&APVPPlayerController::HideScoreboard
			);
		}
	}
}

void APVPPlayerController::OnPlayerDeath()
{
	if (!IsLocalController()) {
		return;
	}

	SetIgnoreMoveInput(true);
}

void APVPPlayerController::OnPawnChanged(
	APawn* const,
	APawn* const
)
{
	if (!IsLocalController()) {
		return;
	}

	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);
}

void APVPPlayerController::ShowScoreboard()
{
	if (!IsLocalController()) {
		return;
	}

	if (myScoreboardWidget) {
		myScoreboardWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APVPPlayerController::HideScoreboard()
{
	if (!IsLocalController()) {
		return;
	}

	if (myScoreboardWidget) {
		myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
