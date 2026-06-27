// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionPlayerController.h"

#include "UIExtractionHUD.h"
#include "ExtractionGameState.h"
#include "Blueprint/UserWidget.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"
#include "ExtractionHUDViewModel.h"

void AExtractionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputGameOnly();
	AddInputMappingContext();
	CreateHUDAndViewModel();
	BindHUDToPawn();
	BindHUDToGameState();
}

void AExtractionPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	BindHUDToPawn();
}

void AExtractionPlayerController::AcknowledgePossession(APawn* aPawn)
{
	Super::AcknowledgePossession(aPawn);
	BindHUDToPawn();
}

void AExtractionPlayerController::CreateHUDAndViewModel()
{
	if (!IsLocalController()) {
		return;
	}

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

void AExtractionPlayerController::BindHUDToPawn()
{
	if (!IsLocalController() || !myHUDViewModel) {
		return;
	}

	APawn* controlledPawn = GetPawn();
	myHUDViewModel->BindToPawn(Cast<AValkPlayerPawn>(controlledPawn));

	UHealthComponent* healthComponent = controlledPawn
		? controlledPawn->FindComponentByClass<UHealthComponent>()
		: nullptr;

	if (myBoundHealthComponent != healthComponent) {
		if (myBoundHealthComponent) {
			myBoundHealthComponent->OnDeath.RemoveDynamic(
				this,
				&AExtractionPlayerController::HandlePlayerDeath
			);
		}

		myBoundHealthComponent = healthComponent;
		if (myBoundHealthComponent) {
			myBoundHealthComponent->OnDeath.AddUniqueDynamic(
				this,
				&AExtractionPlayerController::HandlePlayerDeath
			);
		}
	}
}

void AExtractionPlayerController::BindHUDToGameState()
{
	if (!IsLocalController() || !myHUDViewModel || !GetWorld()) {
		return;
	}

	AExtractionGameState* combatSliceGameState = GetWorld()->GetGameState<AExtractionGameState>();
	myHUDViewModel->BindToGameState(combatSliceGameState);

	if (myBoundExtractionGameState.Get() != combatSliceGameState) {
		myBoundExtractionGameState = combatSliceGameState;
		if (myBoundExtractionGameState.IsValid()) {
			myBoundExtractionGameState->myOnExtractionStateChanged.AddUObject(
				this,
				&AExtractionPlayerController::HandleExtractionStateChanged
			);
			HandleExtractionStateChanged();
		}
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

void AExtractionPlayerController::HandlePlayerDeath()
{
	ShowDeadOverlay();
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
	if (APawn* controlledPawn = GetPawn()) {
		controlledPawn->DisableInput(this);
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
