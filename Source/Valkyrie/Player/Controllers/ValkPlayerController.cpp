// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Valkyrie/GameModes/ValkGameMode.h"

void AValkPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) {
		return;
	}

	// I. input
	// I.1. add input mapping context
	if (myInputMappingContext) {
		if (const ULocalPlayer* const localPlayer = GetLocalPlayer()) {
			if (UEnhancedInputLocalPlayerSubsystem* const inputSubsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
				inputSubsystem->AddMappingContext(myInputMappingContext, 0);
			}
		}
	}
	// I.2. set input mode to game only
	bShowMouseCursor = false;
	const FInputModeGameOnly inputMode;
	SetInputMode(inputMode);

	// II. UI
	// II.1. add HUD to viewport
	if (myHUDWidgetClass) {
		myHUDWidget = CreateWidget<UUserWidget>(this, myHUDWidgetClass);
		if (myHUDWidget) {
			myHUDWidget->AddToViewport();
		}
	}
	// II.2. add scoreboard to viewport
	if (myScoreboardWidgetClass) {
		myScoreboardWidget = CreateWidget<UUserWidget>(this, myScoreboardWidgetClass);
		if (myScoreboardWidget) {
			myScoreboardWidget->AddToViewport();
			myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AValkPlayerController::SetupInputComponent()
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
				&AValkPlayerController::ShowScoreboard
			);
			enhancedInputComponent->BindAction(
				myInputActionOpenScoreboard,
				ETriggerEvent::Completed,
				this,
				&AValkPlayerController::HideScoreboard
			);
		}
	}
}

void AValkPlayerController::OnDied(AController* const aKillerController)
{
	if (!HasAuthority()) {
		return;
	}

	if (const UWorld* const world = GetWorld()) {
		if (AValkGameMode* const gameMode = world->GetAuthGameMode<AValkGameMode>()) {
			gameMode->OnPlayerDied(aKillerController, this);
		}
	}
}

void AValkPlayerController::Client_OnPlayerDied_Implementation()
{
	OnPlayerDied();
}

void AValkPlayerController::Client_OnPlayerRespawned_Implementation()
{
	OnPlayerRespawned();
}

void AValkPlayerController::OnPlayerDied()
{
	if (!IsLocalController()) {
		return;
	}

	SetIgnoreMoveInput(true);
}

void AValkPlayerController::OnPlayerRespawned()
{
	if (!IsLocalController()) {
		return;
	}

	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);
}

void AValkPlayerController::ShowScoreboard()
{
	if (!IsLocalController()) {
		return;
	}

	if (myScoreboardWidget) {
		myScoreboardWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AValkPlayerController::HideScoreboard()
{
	if (!IsLocalController()) {
		return;
	}

	if (myScoreboardWidget) {
		myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
