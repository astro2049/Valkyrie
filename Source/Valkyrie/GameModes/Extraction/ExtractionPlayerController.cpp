// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionPlayerController.h"

#include "UIExtractionHUD.h"
#include "ExtractionGameState.h"
#include "Blueprint/UserWidget.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"
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
	UWeaponComponent* weaponComponent = controlledPawn
		? controlledPawn->FindComponentByClass<UWeaponComponent>()
		: nullptr;
	myHUDViewModel->BindToWeaponComponent(weaponComponent);

	UHealthComponent* healthComponent = controlledPawn
		? controlledPawn->FindComponentByClass<UHealthComponent>()
		: nullptr;
	myHUDViewModel->BindToHealthComponent(healthComponent);
	
	if (UInteractionComponent* interactionComponent = controlledPawn ? controlledPawn->FindComponentByClass<UInteractionComponent>() : nullptr) {
		interactionComponent->OnInteractableChanged.AddUniqueDynamic(
			this,
			&AExtractionPlayerController::HandleInteractableChanged
		);
		HandleInteractableChanged(interactionComponent->HasInteractable());
	}

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
	myHUDViewModel->BindToExtractionGameState(combatSliceGameState);

	if (myBoundCombatSliceGameState != combatSliceGameState) {
		if (myBoundCombatSliceGameState) {
			myBoundCombatSliceGameState->OnCombatSliceStateChanged.RemoveDynamic(
				this,
				&AExtractionPlayerController::HandleCombatSliceStateChanged
			);
		}

		myBoundCombatSliceGameState = combatSliceGameState;
		if (myBoundCombatSliceGameState) {
			myBoundCombatSliceGameState->OnCombatSliceStateChanged.AddUniqueDynamic(
				this,
				&AExtractionPlayerController::HandleCombatSliceStateChanged
			);
			HandleCombatSliceStateChanged(myBoundCombatSliceGameState->GetCombatSliceState());
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

void AExtractionPlayerController::HandleCombatSliceStateChanged(ECombatSliceState aCombatSliceState)
{
	if (aCombatSliceState == ECombatSliceState::Completed) {
		ShowVictoryMenu();
	} else if (aCombatSliceState == ECombatSliceState::Failed) {
		ShowFailureMenu();
	}
}

void AExtractionPlayerController::HandleInteractableChanged(bool aHasInteractable)
{
	if (myHUDViewModel) {
		myHUDViewModel->SetShowInteractPrompt(aHasInteractable);
	}
}
