// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMGameMode.h"

#include "TDMGameState.h"
#include "Valkyrie/Player/States/PVP/TDM/TDMPlayerState.h"
#include "GameFramework/Controller.h"

ATDMGameMode::ATDMGameMode()
{
	GameStateClass = ATDMGameState::StaticClass();
	PlayerStateClass = ATDMPlayerState::StaticClass();
}

void ATDMGameMode::HandleModePlayerKilled(
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
						if (teamKills >= GetScoreLimit()) {
							EndPVPMatch(killerPlayerState->GetTeamId());
						}
					}
				}
			}
		}
	}
}
