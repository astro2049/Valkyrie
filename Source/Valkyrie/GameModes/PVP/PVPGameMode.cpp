// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPGameMode.h"

#include "PVPGameState.h"
#include "Valkyrie/Player/Controllers/PVP/PVPPlayerController.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Valkyrie/Components/HealthComponent.h"

APVPGameMode::APVPGameMode()
{
	GameStateClass = APVPGameState::StaticClass();
	PlayerControllerClass = APVPPlayerController::StaticClass();
	PlayerStateClass = AValkPlayerState::StaticClass();
}

void APVPGameMode::PostLogin(APlayerController* const aNewPlayer)
{
	if (AValkPlayerState* const playerState = aNewPlayer ? aNewPlayer->GetPlayerState<AValkPlayerState>() : nullptr) {
		playerState->SetTeamId(GetBalancedTeamId());
	}

	Super::PostLogin(aNewPlayer);
}

AActor* APVPGameMode::ChoosePlayerStart_Implementation(AController* const aPlayer)
{
	const AValkPlayerState* const playerState = aPlayer ? aPlayer->GetPlayerState<AValkPlayerState>() : nullptr;
	if (!playerState || playerState->GetTeamId() == ValkTeamId::None) {
		return Super::ChoosePlayerStart_Implementation(aPlayer);
	}

	const FName playerStartTag = playerState->GetTeamId() == ValkTeamId::TeamA
		? FName(TEXT("TeamA"))
		: FName(TEXT("TeamB"));

	TArray<AActor*> playerStartActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), playerStartActors);
	TArray<APlayerStart*> teamPlayerStarts;
	for (AActor* const playerStartActor : playerStartActors) {
		if (APlayerStart* const playerStart = Cast<APlayerStart>(playerStartActor);
			playerStart && playerStart->PlayerStartTag == playerStartTag) {
			teamPlayerStarts.Add(playerStart);
		}
	}

	if (teamPlayerStarts.IsEmpty()) {
		return Super::ChoosePlayerStart_Implementation(aPlayer);
	}

	return teamPlayerStarts[FMath::RandHelper(teamPlayerStarts.Num())];
}

void APVPGameMode::RestartPlayer(AController* const aNewPlayer)
{
	Super::RestartPlayer(aNewPlayer);
	BindPlayerDeath(aNewPlayer);
}

void APVPGameMode::EndPVPMatch(const int32 aWinningTeamId)
{
	APVPGameState* const gameState = GetGameState<APVPGameState>();
	if (!gameState || gameState->HasMatchEnded()) {
		return;
	}

	gameState->SetWinningTeamId(aWinningTeamId);
	ScheduleReturnToMainMenu();
}

int32 APVPGameMode::GetBalancedTeamId() const
{
	int32 teamAPlayerCount = 0;
	int32 teamBPlayerCount = 0;
	if (const APVPGameState* const gameState = GetGameState<APVPGameState>()) {
		for (const APlayerState* const playerState : gameState->PlayerArray) {
			const AValkPlayerState* const pvpPlayerState = Cast<AValkPlayerState>(playerState);
			if (pvpPlayerState && pvpPlayerState->GetTeamId() == ValkTeamId::TeamA) {
				++teamAPlayerCount;
			} else if (pvpPlayerState && pvpPlayerState->GetTeamId() == ValkTeamId::TeamB) {
				++teamBPlayerCount;
			}
		}
	}

	return teamAPlayerCount <= teamBPlayerCount ? ValkTeamId::TeamA : ValkTeamId::TeamB;
}

void APVPGameMode::BindPlayerDeath(AController* const aController)
{
	APawn* const pawn = aController ? aController->GetPawn() : nullptr;
	if (UHealthComponent* const healthComponent = pawn ? pawn->FindComponentByClass<UHealthComponent>() : nullptr) {
		healthComponent->myOnHealthDeath.AddUObject(
			this,
			&APVPGameMode::HandlePlayerDeath,
			aController
		);
	}
}

void APVPGameMode::HandlePlayerDeath(AController* const aKillerController, AController* const aVictimController)
{
	const APVPGameState* const gameState = GetGameState<APVPGameState>();
	if (!aVictimController || (gameState && gameState->HasMatchEnded())) {
		return;
	}

	HandleModePlayerKilled(aVictimController, aKillerController);
	if (gameState && gameState->HasMatchEnded()) {
		return;
	}

	FTimerDelegate respawnDelegate;
	respawnDelegate.BindUObject(this, &APVPGameMode::RespawnPlayer, aVictimController);
	FTimerHandle respawnTimerHandle;
	GetWorldTimerManager().SetTimer(
		respawnTimerHandle,
		respawnDelegate,
		myRespawnDelay,
		false
	);
}

void APVPGameMode::RespawnPlayer(AController* const aController)
{
	const APVPGameState* const gameState = GetGameState<APVPGameState>();
	if (!aController || (gameState && gameState->HasMatchEnded())) {
		return;
	}

	APawn* const oldPawn = aController->GetPawn();
	aController->UnPossess();
	if (oldPawn) {
		oldPawn->Destroy();
	}
	RestartPlayer(aController);
}

