// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPPlayerController.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Valkyrie/GameModes/PVP/PVPGameState.h"
#include "Valkyrie/GameModes/PVP/UI/PVPHUDViewModel.h"
#include "Valkyrie/GameModes/PVP/UI/PVPScoreboardViewModel.h"
#include "Valkyrie/GameModes/PVP/UI/UIPVPHUD.h"
#include "Valkyrie/GameModes/PVP/UI/UIPVPScoreboard.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"

void APVPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputGameOnly();
	AddInputMappingContext();
	CreatePVPUI();
	BindUIToSources();
	BindToPawnHealth();
}

void APVPPlayerController::OnPossess(APawn* const aPawn)
{
	Super::OnPossess(aPawn);
	SetIgnoreMoveInput(false);
	BindUIToSources();
	BindToPawnHealth();
}

void APVPPlayerController::AcknowledgePossession(APawn* const aPawn)
{
	Super::AcknowledgePossession(aPawn);
	SetIgnoreMoveInput(false);
	BindUIToSources();
	BindToPawnHealth();
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

void APVPPlayerController::CreatePVPUI()
{
	if (!IsLocalController()) {
		return;
	}

	myHUDViewModel = CreateHUDViewModel();
	myScoreboardViewModel = CreateScoreboardViewModel();
	if (myHUDWidgetClass) {
		myHUDWidget = CreateWidget<UUIPVPHUD>(this, myHUDWidgetClass);
		if (myHUDWidget) {
			myHUDWidget->SetViewModel(myHUDViewModel);
			myHUDWidget->AddToViewport();
		}
	}
	if (myScoreboardWidgetClass) {
		myScoreboardWidget = CreateWidget<UUIPVPScoreboard>(this, myScoreboardWidgetClass);
		if (myScoreboardWidget) {
			myScoreboardWidget->SetViewModel(myScoreboardViewModel);
			myScoreboardWidget->AddToViewport();
			myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APVPPlayerController::BindUIToSources() const
{
	if (!IsLocalController()) {
		return;
	}

	if (myHUDViewModel) {
		myHUDViewModel->BindToPawn(Cast<AValkPlayerPawn>(GetPawn()));
		myHUDViewModel->BindToGameState(GetWorld() ? GetWorld()->GetGameState<APVPGameState>() : nullptr);
	}
	if (myScoreboardViewModel) {
		myScoreboardViewModel->BindToGameState(GetWorld() ? GetWorld()->GetGameState<APVPGameState>() : nullptr);
	}
}

void APVPPlayerController::BindToPawnHealth()
{
	UHealthComponent* const healthComponent = GetPawn()
		? GetPawn()->FindComponentByClass<UHealthComponent>()
		: nullptr;
	if (myBoundHealthComponent == healthComponent) {
		return;
	}

	if (myBoundHealthComponent) {
		myBoundHealthComponent->OnDeath.RemoveDynamic(this, &APVPPlayerController::HandlePlayerDeath);
	}
	myBoundHealthComponent = healthComponent;
	if (myBoundHealthComponent) {
		myBoundHealthComponent->OnDeath.AddUniqueDynamic(this, &APVPPlayerController::HandlePlayerDeath);
	}
}

void APVPPlayerController::HandlePlayerDeath()
{
	SetIgnoreMoveInput(true);
}

void APVPPlayerController::ShowScoreboard()
{
	if (myScoreboardWidget) {
		myScoreboardWidget->RebuildRows();
		myScoreboardWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APVPPlayerController::HideScoreboard()
{
	if (myScoreboardWidget) {
		myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
