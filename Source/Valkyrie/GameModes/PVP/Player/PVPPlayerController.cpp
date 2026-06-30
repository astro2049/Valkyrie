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

	if (myPlayerHUDWidgetClass) {
		myPlayerHUDWidget = CreateWidget<UUserWidget>(this, myPlayerHUDWidgetClass);
		if (myPlayerHUDWidget) {
			myPlayerHUDWidget->AddToViewport();
		}
	}
	if (myModeWidgetClass) {
		myModeWidget = CreateWidget<UUserWidget>(this, myModeWidgetClass);
		if (myModeWidget) {
			myModeWidget->AddToViewport();
		}
	}
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

	if (UEnhancedInputComponent* const enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		if (myScoreboardAction) {
			enhancedInputComponent->BindAction(
				myScoreboardAction,
				ETriggerEvent::Started,
				this,
				&APVPPlayerController::ShowScoreboard
			);
			enhancedInputComponent->BindAction(
				myScoreboardAction,
				ETriggerEvent::Completed,
				this,
				&APVPPlayerController::HideScoreboard
			);
		}
	}
}

void APVPPlayerController::HandleLocalPlayerDeath()
{
	SetIgnoreMoveInput(true);
}

void APVPPlayerController::ShowScoreboard()
{
	if (myScoreboardWidget) {
		myScoreboardWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APVPPlayerController::HideScoreboard()
{
	if (myScoreboardWidget) {
		myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
