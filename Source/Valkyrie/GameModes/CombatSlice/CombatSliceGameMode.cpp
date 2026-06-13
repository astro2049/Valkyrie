// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatSliceGameMode.h"

#include "CombatSliceGameState.h"
#include "TimerManager.h"

ACombatSliceGameMode::ACombatSliceGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACombatSliceGameMode::BeginPlay()
{
	Super::BeginPlay();

	myDefenseTimeRemaining = myDefenseDuration;
	SetCombatSliceState(
		ECombatSliceState::ToStartGenerator,
		FText::FromString(TEXT("Objective: Start the generator"))
	);
	SetDefenseTimer(0.f, false);
}

void ACombatSliceGameMode::StartGenerator()
{
	const ACombatSliceGameState* combatSliceGameState = GetGameState<ACombatSliceGameState>();
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
		&ACombatSliceGameMode::TickDefenseTimer,
		1.f,
		true
	);
}

void ACombatSliceGameMode::CompleteDefense()
{
	const ACombatSliceGameState* combatSliceGameState = GetGameState<ACombatSliceGameState>();
	ensure(combatSliceGameState && combatSliceGameState->GetCombatSliceState() == ECombatSliceState::ToCompleteDefense);
	SetCombatSliceState(
		ECombatSliceState::ToExtract,
		FText::FromString(TEXT("Objective: Reach extraction"))
	);
	SetDefenseTimer(0.f, false);

	GetWorldTimerManager().ClearTimer(myDefenseTimerHandle);
	OnDefenseCompleted();
}

void ACombatSliceGameMode::CompleteExtraction()
{
	const ACombatSliceGameState* combatSliceGameState = GetGameState<ACombatSliceGameState>();
	ensure(combatSliceGameState && combatSliceGameState->GetCombatSliceState() == ECombatSliceState::ToExtract);
	SetCombatSliceState(
		ECombatSliceState::Completed,
		FText::FromString(TEXT("Extraction Complete"))
	);
	SetDefenseTimer(0.f, false);
	OnExtractionCompleted();
}

void ACombatSliceGameMode::TickDefenseTimer()
{
	myDefenseTimeRemaining = FMath::Max(0.f, myDefenseTimeRemaining - 1.f);
	SetDefenseTimer(myDefenseTimeRemaining, true);

	if (myDefenseTimeRemaining <= 0.f) {
		CompleteDefense();
	}
}

void ACombatSliceGameMode::SetCombatSliceState(ECombatSliceState aNewState, const FText& anObjectiveText) const
{
	if (ACombatSliceGameState* combatSliceGameState = GetGameState<ACombatSliceGameState>()) {
		combatSliceGameState->SetCombatSliceState(aNewState);
		combatSliceGameState->SetObjectiveText(anObjectiveText);
	}
}

void ACombatSliceGameMode::SetDefenseTimer(float aDefenseTimeRemaining, bool aShowDefenseTimer) const
{
	if (ACombatSliceGameState* combatSliceGameState = GetGameState<ACombatSliceGameState>()) {
		combatSliceGameState->SetDefenseTimer(aDefenseTimeRemaining, aShowDefenseTimer);
	}
}
