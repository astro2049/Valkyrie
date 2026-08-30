// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Valkyrie/GameModes/ValkGameMode.h"
#include "Valkyrie/UI/DamageIndicatorInterface.h"
#include "Valkyrie/UI/UIMessageSubsystem.h"

void AValkPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// setup input and UI
	if (IsLocalController()) {
		// I. input
		// I.1. add input mapping context
		GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(myInputMappingContext, 0);
		// I.2. set input mode to game only
		bShowMouseCursor = false;
		const FInputModeGameOnly inputMode;
		SetInputMode(inputMode);

		// II. UI
		// II.1. HUD
		myHUDWidget = CreateWidget<UUserWidget>(this, myHUDWidgetClass);
		myHUDWidget->AddToViewport();
		// II.2. scoreboard
		myScoreboardWidget = CreateWidget<UUserWidget>(this, myScoreboardWidgetClass);
		myScoreboardWidget->AddToViewport();
		myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AValkPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalController()) {
		UEnhancedInputComponent* const enhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
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

void AValkPlayerController::OnControlledPawnDied(AController* const aKillerController)
{
	GetWorld()->GetAuthGameMode<AValkGameMode>()->PlayerDied(aKillerController, this);
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
	SetIgnoreMoveInput(true);
}

void AValkPlayerController::OnPlayerRespawned()
{
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);
}

void AValkPlayerController::ShowScoreboard()
{
	myScoreboardWidget->SetVisibility(ESlateVisibility::Visible);
}

void AValkPlayerController::HideScoreboard()
{
	myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AValkPlayerController::Client_PlayHitRepresentations_Implementation()
{
	VALK_UIMESSAGESUBSYS()->BroadcastUIMessage(UIMessage::LocalPlayerHitConfirmed);
}

void AValkPlayerController::Client_PlayDamageRepresentations_Implementation(const FVector aDamageSourceLocation)
{
	IDamageIndicatorInterface::Execute_PlayDamageIndicator(myHUDWidget, aDamageSourceLocation);
}
