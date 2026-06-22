// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionZone.h"

#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameMode.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"

namespace
{
	const FName ExtractionTriggerTag{TEXT("ExtractionTriggerBox")};
}

AExtractionZone::AExtractionZone()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorHiddenInGame(true);
}

void AExtractionZone::BeginPlay()
{
	Super::BeginPlay();

	const TArray<UActorComponent*> taggedComponents = GetComponentsByTag(
		UBoxComponent::StaticClass(),
		ExtractionTriggerTag
	);
	myBoxComponent = taggedComponents.IsEmpty() ? nullptr : Cast<UBoxComponent>(taggedComponents[0]);
	if (!ensureMsgf(myBoxComponent, TEXT("Extraction Zone requires a Box Component tagged ExtractionTriggerBox."))) {
		return;
	}

	myBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	myBoxComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&AExtractionZone::HandleBeginOverlap
	);
}

void AExtractionZone::Activate()
{
	if (!myBoxComponent) {
		return;
	}

	SetActorHiddenInGame(false);
	myBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AExtractionZone::HandleBeginOverlap(
	UPrimitiveComponent* anOverlappedComponent,
	AActor* anOtherActor,
	UPrimitiveComponent* anOtherComponent,
	int32 anOtherBodyIndex,
	bool aFromSweep,
	const FHitResult& aSweepResult)
{
	if (!Cast<AValkPlayerPawn>(anOtherActor)) {
		return;
	}

	if (AExtractionGameMode* extractionGameMode = GetWorld()->GetAuthGameMode<AExtractionGameMode>()) {
		extractionGameMode->CompleteExtraction();
	}
}
