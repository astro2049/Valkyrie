// Fill out your copyright notice in the Description page of Project Settings.

#include "PVPGameMode.h"

#include "Valkyrie/GameModes/ValkGameState.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

APVPGameMode::APVPGameMode()
{
	PlayerControllerClass = AValkPlayerController::StaticClass();
}

void APVPGameMode::PostLogin(APlayerController* const aNewPlayer)
{
	if (aNewPlayer) {
		if (AValkPlayerState* const playerState = aNewPlayer->GetPlayerState<AValkPlayerState>()) {
			playerState->SetTeamId(GetBalancedTeamId());
		}
	}

	Super::PostLogin(aNewPlayer);
}

void APVPGameMode::EndPVPMatch(const EValkTeamId aWinningTeamId)
{
	AValkGameState* const gameState = GetGameState<AValkGameState>();
	if (gameState && !gameState->HasMatchEnded()) {
		gameState->SetWinningTeamId(aWinningTeamId);
		ScheduleReturnToMainMenu();
	}
}

EValkTeamId APVPGameMode::GetBalancedTeamId() const
{
	int32 teamAPlayerCount = 0;
	int32 teamBPlayerCount = 0;
	if (const AValkGameState* const gameState = GetGameState<AValkGameState>()) {
		for (const APlayerState* const playerState : gameState->PlayerArray) {
			const AValkPlayerState* const pvpPlayerState = Cast<AValkPlayerState>(playerState);
			if (pvpPlayerState && pvpPlayerState->GetTeamId() == EValkTeamId::TeamA) {
				++teamAPlayerCount;
			} else if (pvpPlayerState && pvpPlayerState->GetTeamId() == EValkTeamId::TeamB) {
				++teamBPlayerCount;
			}
		}
	}

	return teamAPlayerCount <= teamBPlayerCount ? EValkTeamId::TeamA : EValkTeamId::TeamB;
}

void APVPGameMode::OnPlayerDeath(AController* const aKillerController, AController* const aVictimController)
{
	if (!aVictimController) {
		return;
	}

	Super::OnPlayerDeath(aKillerController, aVictimController);

	const AValkGameState* const gameState = GetGameState<AValkGameState>();
	if (!gameState || !gameState->HasMatchEnded()) {
		HandleModePlayerKilled(aVictimController, aKillerController);
		if (!gameState || !gameState->HasMatchEnded()) {
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
	}
}

void APVPGameMode::RespawnPlayer(AController* const aController)
{
	if (!aController) {
		return;
	}

	const AValkGameState* const gameState = GetGameState<AValkGameState>();
	if (!gameState || !gameState->HasMatchEnded()) {
		APawn* const oldPawn = aController->GetPawn();
		aController->UnPossess();
		if (oldPawn) {
			oldPawn->Destroy();
		}
		RestartPlayer(aController);
	}
}

