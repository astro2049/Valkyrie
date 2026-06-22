// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionGameMode.h"

#include "Actors/ExtractionZone.h"
#include "ExtractionGameState.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AExtractionGameMode::AExtractionGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AExtractionGameMode::BeginPlay()
{
	Super::BeginPlay();

	myExtractionZone = Cast<AExtractionZone>(
		UGameplayStatics::GetActorOfClass(this, AExtractionZone::StaticClass())
	);

	myDefenseTimeRemaining = myDefenseDuration;
	SetCombatSliceState(
		ECombatSliceState::ToStartGenerator,
		FText::FromString(TEXT("Objective: Start the generator"))
	);
	SetDefenseTimer(0.f, false);
}

void AExtractionGameMode::StartGenerator()
{
	const AExtractionGameState* combatSliceGameState = GetGameState<AExtractionGameState>();
	ensure(combatSliceGameState && combatSliceGameState->GetCombatSliceState() == ECombatSliceState::ToStartGenerator);
	SetCombatSliceState(
		ECombatSliceState::ToCompleteDefense,
		FText::FromString(TEXT("Objective: Defend the generator"))
	);

	myDefenseTimeRemaining = myDefenseDuration;
	SetDefenseTimer(myDefenseTimeRemaining, true);
	GetWorldTimerManager().SetTimer(
		myDefenseTimerHandle,
		this,
		&AExtractionGameMode::TickDefenseTimer,
		1.f,
		true
	);
}

void AExtractionGameMode::CompleteDefense()
{
	const AExtractionGameState* combatSliceGameState = GetGameState<AExtractionGameState>();
	ensure(combatSliceGameState && combatSliceGameState->GetCombatSliceState() == ECombatSliceState::ToCompleteDefense);
	SetCombatSliceState(
		ECombatSliceState::ToExtract,
		FText::FromString(TEXT("Objective: Reach extraction"))
	);
	SetDefenseTimer(0.f, false);

	GetWorldTimerManager().ClearTimer(myDefenseTimerHandle);
	if (AExtractionZone* extractionZone = myExtractionZone.Get()) {
		extractionZone->Activate();
	}
}

void AExtractionGameMode::CompleteExtraction()
{
	const AExtractionGameState* combatSliceGameState = GetGameState<AExtractionGameState>();
	ensure(combatSliceGameState && combatSliceGameState->GetCombatSliceState() == ECombatSliceState::ToExtract);
	SetCombatSliceState(
		ECombatSliceState::Completed,
		FText::FromString(TEXT("Extraction Complete"))
	);
	SetDefenseTimer(0.f, false);
	OnExtractionCompleted();
}

void AExtractionGameMode::TickDefenseTimer()
{
	myDefenseTimeRemaining = FMath::Max(0.f, myDefenseTimeRemaining - 1.f);
	SetDefenseTimer(myDefenseTimeRemaining, true);

	if (myDefenseTimeRemaining <= 0.f) {
		CompleteDefense();
	}
}

void AExtractionGameMode::SetCombatSliceState(ECombatSliceState aNewState, const FText& anObjectiveText) const
{
	if (AExtractionGameState* combatSliceGameState = GetGameState<AExtractionGameState>()) {
		combatSliceGameState->SetCombatSliceState(aNewState);
		combatSliceGameState->SetObjectiveText(anObjectiveText);
	}
}

void AExtractionGameMode::SetDefenseTimer(float aDefenseTimeRemaining, bool aShowDefenseTimer) const
{
	if (AExtractionGameState* combatSliceGameState = GetGameState<AExtractionGameState>()) {
		combatSliceGameState->SetDefenseTimer(aDefenseTimeRemaining, aShowDefenseTimer);
	}
}
