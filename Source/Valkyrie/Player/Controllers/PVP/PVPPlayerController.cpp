// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Valkyrie/Components/HealthComponent.h"

void APVPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) {
		return;
	}

	OnPossessedPawnChanged.AddUniqueDynamic(
		this,
		&APVPPlayerController::HandlePossessedPawnChanged
	);
	SetControlledPawn(GetPawn());

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

void APVPPlayerController::SetControlledPawn(const APawn* const aPawn)
{
	if (!IsLocalController()) {
		return;
	}

	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);

	if (myHealthComponent) {
		myHealthComponent->OnDeath.RemoveDynamic(
			this,
			&APVPPlayerController::HandlePlayerDied
		);
	}
	myHealthComponent = aPawn ? aPawn->FindComponentByClass<UHealthComponent>() : nullptr;
	if (myHealthComponent) {
		myHealthComponent->OnDeath.AddUniqueDynamic(
			this,
			&APVPPlayerController::HandlePlayerDied
		);
	}
}

void APVPPlayerController::HandlePossessedPawnChanged(
	APawn* const,
	APawn* const aNewPawn
)
{
	SetControlledPawn(aNewPawn);
}

void APVPPlayerController::HandlePlayerDied()
{
	if (!IsLocalController()) {
		return;
	}

	SetIgnoreMoveInput(true);
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
