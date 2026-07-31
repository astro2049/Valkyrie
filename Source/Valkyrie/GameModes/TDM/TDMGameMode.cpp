// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMGameMode.h"

#include "TDMGameState.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "Valkyrie/GameModes/ValkGameState.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "Valkyrie/Player/States/TDM/TDMPlayerState.h"

ATDMGameMode::ATDMGameMode()
{
	GameStateClass = ATDMGameState::StaticClass();
	PlayerControllerClass = AValkPlayerController::StaticClass();
	PlayerStateClass = ATDMPlayerState::StaticClass();
	myTeamCount = 2;
}

void ATDMGameMode::PlayerDied(AController* const aKillerController, AController* const aVictimController)
{
	if (!aVictimController) {
		return;
	}

	Super::PlayerDied(aKillerController, aVictimController);

	bool canHandleDeath = true;
	if (const AValkGameState* const gameState = GetGameState<AValkGameState>()) {
		canHandleDeath = !gameState->HasMatchEnded();
	}

	if (canHandleDeath) {
		HandlePlayerKilled(aVictimController, aKillerController);
		bool canRespawn = true;
		if (const AValkGameState* const gameState = GetGameState<AValkGameState>()) {
			canRespawn = !gameState->HasMatchEnded();
		}
		if (canRespawn) {
			FTimerDelegate respawnDelegate;
			respawnDelegate.BindUObject(this, &ATDMGameMode::RespawnPlayer, aVictimController);
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

void ATDMGameMode::HandlePlayerKilled(
	AController* const aVictimController,
	AController* const aKillerController
)
{
	if (!aVictimController) {
		return;
	}

	if (ATDMPlayerState* const victimPlayerState = aVictimController->GetPlayerState<ATDMPlayerState>()) {
		victimPlayerState->AddDeath();
		if (aKillerController) {
			if (ATDMPlayerState* const killerPlayerState = aKillerController->GetPlayerState<ATDMPlayerState>()) {
				if (victimPlayerState != killerPlayerState
					&& victimPlayerState->GetTeamId() != killerPlayerState->GetTeamId()) {
					killerPlayerState->AddKill();
					if (ATDMGameState* const gameState = GetGameState<ATDMGameState>()) {
						const int32 teamKills = gameState->AddTeamKill(killerPlayerState->GetTeamId());
						if (teamKills >= myScoreLimit) {
							EndTDMMatch(killerPlayerState->GetTeamId());
						}
					}
				}
			}
		}
	}
}

void ATDMGameMode::EndTDMMatch(const EValkTeamId aWinningTeamId)
{
	if (AValkGameState* const gameState = GetGameState<AValkGameState>()) {
		gameState->SetWinningTeamId(aWinningTeamId);
		FinishMatch();
	}
}

void ATDMGameMode::RespawnPlayer(AController* const aController)
{
	if (!aController) {
		return;
	}

	bool canRespawn = true;
	if (const AValkGameState* const gameState = GetGameState<AValkGameState>()) {
		canRespawn = !gameState->HasMatchEnded();
	}

	if (canRespawn) {
		APawn* const oldPawn = aController->GetPawn();
		aController->UnPossess();
		if (oldPawn) {
			oldPawn->Destroy();
		}
		RestartPlayer(aController);
		if (AValkPlayerController* const playerController = Cast<AValkPlayerController>(aController)) {
			playerController->Client_OnPlayerRespawned();
		}
	}
}
