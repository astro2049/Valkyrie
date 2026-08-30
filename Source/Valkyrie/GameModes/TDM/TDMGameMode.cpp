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
	Super::PlayerDied(aKillerController, aVictimController);

	if (!GetGameState<AValkGameState>()->HasMatchEnded()) {
		HandlePlayerKilled(aVictimController, aKillerController);
		if (!GetGameState<AValkGameState>()->HasMatchEnded()) {
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
	ATDMPlayerState* const victimPlayerState = aVictimController->GetPlayerState<ATDMPlayerState>();
	victimPlayerState->AddDeath();
	if (aKillerController) {
		ATDMPlayerState* const killerPlayerState = aKillerController->GetPlayerState<ATDMPlayerState>();
		if (victimPlayerState != killerPlayerState && victimPlayerState->GetTeamId() != killerPlayerState->GetTeamId()) {
			killerPlayerState->AddKill();
			const int32 teamKills = GetGameState<ATDMGameState>()->AddTeamKill(killerPlayerState->GetTeamId());
			if (teamKills >= myScoreLimit) {
				EndTDMMatch(killerPlayerState->GetTeamId());
			}
		}
	}
}

void ATDMGameMode::EndTDMMatch(const EValkTeamId aWinningTeamId)
{
	GetGameState<AValkGameState>()->SetWinningTeamId(aWinningTeamId);
	FinishMatch();
}

void ATDMGameMode::RespawnPlayer(AController* const aController)
{
	if (!GetGameState<AValkGameState>()->HasMatchEnded()) {
		APawn* const oldPawn = aController->GetPawn();
		aController->UnPossess();
		if (oldPawn) {
			oldPawn->Destroy();
		}
		RestartPlayer(aController);
		CastChecked<AValkPlayerController>(aController)->Client_OnPlayerRespawned();
	}
}
