// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionZone.h"

#include "Engine/World.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameMode.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"

AExtractionZone::AExtractionZone()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AExtractionZone::HandlePlayerEntered(AActor* anOtherActor)
{
	if (!Cast<AValkPlayerPawn>(anOtherActor)) {
		return;
	}

	if (AExtractionGameMode* extractionGameMode = GetWorld()->GetAuthGameMode<AExtractionGameMode>()) {
		extractionGameMode->CompleteExtraction();
	}
}
