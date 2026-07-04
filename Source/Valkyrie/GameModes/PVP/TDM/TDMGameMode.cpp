// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMGameMode.h"

#include "TDMGameState.h"
#include "Valkyrie/Player/Controllers/PVP/TDM/TDMPlayerController.h"
#include "Valkyrie/Player/States/PVP/TDM/TDMPlayerState.h"
#include "GameFramework/Controller.h"

ATDMGameMode::ATDMGameMode()
{
	GameStateClass = ATDMGameState::StaticClass();
	PlayerControllerClass = ATDMPlayerController::StaticClass();
	PlayerStateClass = ATDMPlayerState::StaticClass();
}

void ATDMGameMode::HandleModePlayerKilled(
	AController* const aVictimController,
	AController* const aKillerController
)
{
	ATDMPlayerState* const victimPlayerState = aVictimController
		? aVictimController->GetPlayerState<ATDMPlayerState>()
		: nullptr;
	if (victimPlayerState) {
		victimPlayerState->AddDeath();
	}

	ATDMPlayerState* const killerPlayerState = aKillerController
		? aKillerController->GetPlayerState<ATDMPlayerState>()
		: nullptr;
	if (!victimPlayerState || !killerPlayerState || victimPlayerState == killerPlayerState
		|| victimPlayerState->GetTeamId() == killerPlayerState->GetTeamId()) {
		return;
	}

	killerPlayerState->AddKill();
	if (ATDMGameState* const gameState = GetGameState<ATDMGameState>()) {
		const int32 teamKills = gameState->AddTeamKill(killerPlayerState->GetTeamId());
		if (teamKills >= GetScoreLimit()) {
			EndPVPMatch(killerPlayerState->GetTeamId());
		}
	}
}
