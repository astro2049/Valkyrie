// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionGameMode.h"

#include "Valkyrie/Actors/Extraction/ExtractionZone.h"
#include "ExtractionGameState.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Valkyrie/Components/HealthComponent.h"

AExtractionGameMode::AExtractionGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	GameStateClass = AExtractionGameState::StaticClass();
}

void AExtractionGameMode::BeginPlay()
{
	Super::BeginPlay();

	myExtractionZone = Cast<AExtractionZone>(
		UGameplayStatics::GetActorOfClass(this, AExtractionZone::StaticClass())
	);

	myDefenseTimeRemaining = myDefenseDuration;
	SetCombatSliceState(ECombatSliceState::ToStartGenerator);
	SetDefenseTimeRemaining(0.f);
}

void AExtractionGameMode::StartGenerator()
{
	const AExtractionGameState* combatSliceGameState = GetGameState<AExtractionGameState>();
	ensure(combatSliceGameState && combatSliceGameState->GetCombatSliceState() == ECombatSliceState::ToStartGenerator);
	SetCombatSliceState(ECombatSliceState::ToCompleteDefense);

	myDefenseTimeRemaining = myDefenseDuration;
	SetDefenseTimeRemaining(myDefenseTimeRemaining);
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
	SetCombatSliceState(ECombatSliceState::ToExtract);
	SetDefenseTimeRemaining(0.f);

	GetWorldTimerManager().ClearTimer(myDefenseTimerHandle);
	if (AExtractionZone* extractionZone = myExtractionZone.Get()) {
		extractionZone->ActivateForAll();
	}
}

void AExtractionGameMode::CompleteExtraction()
{
	const AExtractionGameState* combatSliceGameState = GetGameState<AExtractionGameState>();
	ensure(combatSliceGameState && combatSliceGameState->GetCombatSliceState() == ECombatSliceState::ToExtract);
	SetCombatSliceState(ECombatSliceState::Completed);
	SetDefenseTimeRemaining(0.f);
	ScheduleReturnToMainMenu();
}

void AExtractionGameMode::OnPlayerDied(
	AController* const aKillerController,
	AController* const aVictimController
)
{
	Super::OnPlayerDied(aKillerController, aVictimController);

	if (AreAllPlayersDead()) {
		FailExtraction();
	}
}

void AExtractionGameMode::TickDefenseTimer()
{
	myDefenseTimeRemaining = FMath::Max(0.f, myDefenseTimeRemaining - 1.f);
	SetDefenseTimeRemaining(myDefenseTimeRemaining);

	if (myDefenseTimeRemaining <= 0.f) {
		CompleteDefense();
	}
}

bool AExtractionGameMode::AreAllPlayersDead() const
{
	bool hasPlayer = false;
	bool areAllPlayersDead = true;
	for (FConstPlayerControllerIterator playerControllerIterator = GetWorld()->GetPlayerControllerIterator(); playerControllerIterator; ++playerControllerIterator) {
		if (const APlayerController* playerController = playerControllerIterator->Get()) {
			if (const APawn* pawn = playerController->GetPawn()) {
				if (const UHealthComponent* healthComponent = pawn->FindComponentByClass<UHealthComponent>()) {
					hasPlayer = true;
					if (!healthComponent->IsDead()) {
						areAllPlayersDead = false;
						break;
					}
				} else {
					areAllPlayersDead = false;
					break;
				}
			}
		}
	}

	return hasPlayer && areAllPlayersDead;
}

void AExtractionGameMode::FailExtraction()
{
	const AExtractionGameState* combatSliceGameState = GetGameState<AExtractionGameState>();
	if (combatSliceGameState
		&& combatSliceGameState->GetCombatSliceState() != ECombatSliceState::Completed
		&& combatSliceGameState->GetCombatSliceState() != ECombatSliceState::Failed) {
		SetCombatSliceState(ECombatSliceState::Failed);
		SetDefenseTimeRemaining(0.f);
		GetWorldTimerManager().ClearTimer(myDefenseTimerHandle);
		ScheduleReturnToMainMenu();
	}
}

void AExtractionGameMode::SetCombatSliceState(const ECombatSliceState aNewState) const
{
	if (AExtractionGameState* combatSliceGameState = GetGameState<AExtractionGameState>()) {
		combatSliceGameState->SetCombatSliceState(aNewState);
	}
}

void AExtractionGameMode::SetDefenseTimeRemaining(const float aDefenseTimeRemaining) const
{
	if (AExtractionGameState* combatSliceGameState = GetGameState<AExtractionGameState>()) {
		combatSliceGameState->SetDefenseTimeRemaining(aDefenseTimeRemaining);
	}
}
