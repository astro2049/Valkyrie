// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionGameMode.h"

#include "Actors/ExtractionZone.h"
#include "ExtractionGameState.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Valkyrie/Components/HealthComponent.h"

AExtractionGameMode::AExtractionGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

AActor* AExtractionGameMode::ChoosePlayerStart_Implementation(AController* aPlayer)
{
	TArray<AActor*> playerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), playerStarts);
	if (playerStarts.IsEmpty()) {
		return Super::ChoosePlayerStart_Implementation(aPlayer);
	}

	playerStarts.Sort([](const AActor& aLeft, const AActor& aRight) {
		return aLeft.GetName() < aRight.GetName();
	});

	int32 playerIndex = 0;
	for (FConstPlayerControllerIterator playerControllerIterator = GetWorld()->GetPlayerControllerIterator(); playerControllerIterator; ++playerControllerIterator) {
		if (playerControllerIterator->Get() == aPlayer) {
			break;
		}
		++playerIndex;
	}

	return playerStarts[playerIndex % playerStarts.Num()];
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

void AExtractionGameMode::RestartPlayer(AController* aNewPlayer)
{
	Super::RestartPlayer(aNewPlayer);
	BindPlayerDeath(aNewPlayer);
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
}

void AExtractionGameMode::HandlePlayerDeath()
{
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

void AExtractionGameMode::BindPlayerDeath(AController* aController)
{
	APawn* pawn = aController ? aController->GetPawn() : nullptr;
	UHealthComponent* healthComponent = pawn ? pawn->FindComponentByClass<UHealthComponent>() : nullptr;
	if (healthComponent) {
		healthComponent->OnDeath.AddUniqueDynamic(
			this,
			&AExtractionGameMode::HandlePlayerDeath
		);
	}
}

bool AExtractionGameMode::AreAllPlayersDead() const
{
	bool hasPlayer = false;
	for (FConstPlayerControllerIterator playerControllerIterator = GetWorld()->GetPlayerControllerIterator(); playerControllerIterator; ++playerControllerIterator) {
		const APlayerController* playerController = playerControllerIterator->Get();
		const APawn* pawn = playerController ? playerController->GetPawn() : nullptr;
		const UHealthComponent* healthComponent = pawn ? pawn->FindComponentByClass<UHealthComponent>() : nullptr;
		if (healthComponent) {
			hasPlayer = true;
			if (!healthComponent->IsDead()) {
				return false;
			}
		} else {
			return false;
		}
	}

	return hasPlayer;
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
