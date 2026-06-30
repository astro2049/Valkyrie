// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkGameplayPlayerController.h"

#include "Valkyrie/Components/HealthComponent.h"

void AValkGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) {
		return;
	}

	SetInputGameOnly();
	AddInputMappingContext();
	OnPossessedPawnChanged.AddUniqueDynamic(
		this,
		&AValkGameplayPlayerController::HandlePossessedPawnChanged
	);
	SetControlledPawn(GetPawn());
}

void AValkGameplayPlayerController::SetControlledPawn(APawn* const aPawn)
{
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);

	if (myBoundHealthComponent) {
		myBoundHealthComponent->OnDeath.RemoveDynamic(
			this,
			&AValkGameplayPlayerController::HandleHealthDeath
		);
	}
	myBoundHealthComponent = aPawn ? aPawn->FindComponentByClass<UHealthComponent>() : nullptr;
	if (myBoundHealthComponent) {
		myBoundHealthComponent->OnDeath.AddUniqueDynamic(
			this,
			&AValkGameplayPlayerController::HandleHealthDeath
		);
	}

}

void AValkGameplayPlayerController::HandlePossessedPawnChanged(
	APawn* const,
	APawn* const aNewPawn
)
{
	SetControlledPawn(aNewPawn);
}

void AValkGameplayPlayerController::HandleHealthDeath()
{
	HandleLocalPlayerDeath();
}
