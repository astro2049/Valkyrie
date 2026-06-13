// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatSlicePlayerController.h"

void ACombatSlicePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;

	FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
}
