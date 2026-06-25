// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionZone.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameMode.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"

AExtractionZone::AExtractionZone()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AExtractionZone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExtractionZone, myIsActive);
}

void AExtractionZone::ActivateForAll()
{
	if (HasAuthority() && !myIsActive) {
		myIsActive = true;
		Activate();
	}
}

void AExtractionZone::OnRep_IsActive()
{
	if (myIsActive) {
		Activate();
	}
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
