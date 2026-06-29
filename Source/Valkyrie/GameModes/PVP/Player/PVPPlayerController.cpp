// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPPlayerController.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Valkyrie/GameModes/PVP/UI/UIPVPHUD.h"
#include "Valkyrie/GameModes/PVP/UI/UIPVPScoreboard.h"
#include "Valkyrie/UI/UIValkHUD.h"

void APVPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) {
		return;
	}

	if (myPlayerHUDWidgetClass) {
		myPlayerHUDWidget = CreateWidget<UUIValkHUD>(this, myPlayerHUDWidgetClass);
		if (myPlayerHUDWidget) {
			myPlayerHUDWidget->AddToViewport();
		}
	}
	if (myModeWidgetClass) {
		myModeWidget = CreateWidget<UUIPVPHUD>(this, myModeWidgetClass);
		if (myModeWidget) {
			myModeWidget->AddToViewport();
		}
	}
	if (myScoreboardWidgetClass) {
		myScoreboardWidget = CreateWidget<UUIPVPScoreboard>(this, myScoreboardWidgetClass);
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
		myScoreboardWidget->RefreshScoreboardData();
		myScoreboardWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APVPPlayerController::HideScoreboard()
{
	if (myScoreboardWidget) {
		myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
