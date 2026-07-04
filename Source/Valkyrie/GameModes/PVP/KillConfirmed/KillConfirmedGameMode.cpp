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

bool AKillConfirmedGameMode::ConfirmTag(APawn* const aPlayerPawn)
{
	AKillConfirmedGameState* const gameState = GetGameState<AKillConfirmedGameState>();
	AKillConfirmedPlayerState* const playerState = aPlayerPawn && aPlayerPawn->GetController()
		? aPlayerPawn->GetController()->GetPlayerState<AKillConfirmedPlayerState>()
		: nullptr;
	if (!gameState || gameState->HasMatchEnded() || !playerState) {
		return false;
	}

	playerState->AddConfirm();
	const int32 teamConfirms = gameState->AddTeamConfirm(playerState->GetTeamId());
	if (teamConfirms >= GetScoreLimit()) {
		EndPVPMatch(playerState->GetTeamId());
	}
	return true;
}

void AKillConfirmedGameMode::HandleModePlayerKilled(
	AController* const aVictimController,
	AController* const aKillerController
)
{
	AKillConfirmedPlayerState* const victimPlayerState = aVictimController
		? aVictimController->GetPlayerState<AKillConfirmedPlayerState>()
		: nullptr;
	if (victimPlayerState) {
		victimPlayerState->AddDeath();
	}

	AKillConfirmedPlayerState* const killerPlayerState = aKillerController
		? aKillerController->GetPlayerState<AKillConfirmedPlayerState>()
		: nullptr;
	if (!victimPlayerState || !killerPlayerState || victimPlayerState == killerPlayerState
		|| victimPlayerState->GetTeamId() == killerPlayerState->GetTeamId()) {
		return;
	}

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
