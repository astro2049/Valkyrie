// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneratorInteractableComponent.h"

#include "Engine/World.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameMode.h"

void UGeneratorInteractableComponent::Interact()
{
	if (AExtractionGameMode* extractionGameMode = GetWorld()->GetAuthGameMode<AExtractionGameMode>()) {
		UE_LOG(LogTemp, Log, TEXT("UGeneratorInteractableComponent::Interact"));
		extractionGameMode->StartGenerator();
	}
}
