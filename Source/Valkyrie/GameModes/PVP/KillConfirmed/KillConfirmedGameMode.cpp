// Fill out your copyright notice in the Description page of Project Settings.

#include "KillConfirmedGameMode.h"

#include "KillConfirmedGameState.h"
#include "Valkyrie/Player/Controllers/PVP/KillConfirmed/KillConfirmedPlayerController.h"
#include "Valkyrie/Player/States/PVP/KillConfirmed/KillConfirmedPlayerState.h"
#include "Valkyrie/Actors/PVP/KillConfirmed/KillConfirmedTag.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

AKillConfirmedGameMode::AKillConfirmedGameMode()
{
	GameStateClass = AKillConfirmedGameState::StaticClass();
	PlayerControllerClass = AKillConfirmedPlayerController::StaticClass();
	PlayerStateClass = AKillConfirmedPlayerState::StaticClass();
}

bool AKillConfirmedGameMode::ConfirmTag(const APawn* const aPlayerPawn)
{
	if (!aPlayerPawn) {
		return false;
	}

	bool hasConfirmed = false;
	AKillConfirmedGameState* const gameState = GetGameState<AKillConfirmedGameState>();
	if (const AController* const playerController = aPlayerPawn->GetController()) {
		if (AKillConfirmedPlayerState* const playerState = playerController->GetPlayerState<AKillConfirmedPlayerState>()) {
			if (gameState && !gameState->HasMatchEnded()) {
				playerState->AddConfirm();
				const int32 teamConfirms = gameState->AddTeamConfirm(playerState->GetTeamId());
				if (teamConfirms >= GetScoreLimit()) {
					EndPVPMatch(playerState->GetTeamId());
				}
				hasConfirmed = true;
			}
		}
	}
	return hasConfirmed;
}

void AKillConfirmedGameMode::HandleModePlayerKilled(
	AController* const aVictimController,
	AController* const aKillerController
)
{
	if (!aVictimController) {
		return;
	}

	if (AKillConfirmedPlayerState* const victimPlayerState = aVictimController->GetPlayerState<AKillConfirmedPlayerState>()) {
		victimPlayerState->AddDeath();
		if (aKillerController) {
			if (AKillConfirmedPlayerState* const killerPlayerState = aKillerController->GetPlayerState<AKillConfirmedPlayerState>()) {
				if (victimPlayerState != killerPlayerState
					&& victimPlayerState->GetTeamId() != killerPlayerState->GetTeamId()) {
					killerPlayerState->AddKill();
					if (myTagClass && aVictimController->GetPawn()) {
						if (AKillConfirmedTag* const tag = GetWorld()->SpawnActor<AKillConfirmedTag>(
							myTagClass,
							aVictimController->GetPawn()->GetActorTransform()
						)) {
							tag->SetDroppedTeamId(victimPlayerState->GetTeamId());
						}
					}
				}
			}
		}
	}
}
