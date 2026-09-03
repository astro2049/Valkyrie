// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerController.h"

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
		// I. UI
		// hud
		myHUDWidget = CreateWidget<UUserWidget>(this, myHUDWidgetClass);
		myHUDWidget->AddToViewport();
		// scoreboard
		myScoreboardWidget = CreateWidget<UUserWidget>(this, myScoreboardWidgetClass);
		myScoreboardWidget->AddToViewport();
		myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
		// esc menu
		myEscMenuWidget = CreateWidget<UUserWidget>(this, myEscMenuWidgetClass);
		myEscMenuWidget->AddToViewport();
		myEscMenuWidget->SetVisibility(ESlateVisibility::Hidden);

		// II. input
		SetInputModeGameOnly();
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
		enhancedInputComponent->BindAction(
			myInputActionEscMenuOpen,
			ETriggerEvent::Completed,
			this,
			&AValkPlayerController::OpenEscMenu
		);
		enhancedInputComponent->BindAction(
			myInputActionEscMenuClose,
			ETriggerEvent::Completed,
			this,
			&AValkPlayerController::CloseEscMenu
		);
	}
}

void AValkPlayerController::SetInputModeUIOnly()
{
	GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->RemoveMappingContext(myGameplayInputMappingContext);
	GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(myUIInputMappingContext, 0);
	bShowMouseCursor = true;
	const FInputModeGameAndUI inputMode;
	SetInputMode(inputMode);
}

void AValkPlayerController::SetInputModeGameOnly()
{
	GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->RemoveMappingContext(myUIInputMappingContext);
	GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(myGameplayInputMappingContext, 0);
	bShowMouseCursor = false;
	const FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
}

void AValkPlayerController::OpenEscMenu()
{
	myEscMenuWidget->SetVisibility(ESlateVisibility::Visible);
	SetInputModeUIOnly();
}

void AValkPlayerController::CloseEscMenu()
{
	myEscMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	SetInputModeGameOnly();
}

void AValkPlayerController::ReturnToMainMenu()
{
	if (GetNetMode() < NM_Client) {
		GetWorld()->GetAuthGameMode()->ReturnToMainMenuHost();
	} else {
		ClientReturnToMainMenuWithTextReason(FText::GetEmpty());
	}
}

void AValkPlayerController::Client_OnPlayerDied_Implementation()
{
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
}

void AValkPlayerController::Client_OnPlayerRespawned_Implementation()
{
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);
}

void AValkPlayerController::Client_PlayHitRepresentations_Implementation()
{
	VALK_UIMESSAGESUBSYS()->BroadcastUIMessage(UIMessage::LocalPlayerHitConfirmed);
}

void AValkPlayerController::Client_PlayDamageRepresentations_Implementation(const FVector aDamageSourceLocation)
{
	IDamageIndicatorInterface::Execute_PlayDamageIndicator(myHUDWidget, aDamageSourceLocation);
}
