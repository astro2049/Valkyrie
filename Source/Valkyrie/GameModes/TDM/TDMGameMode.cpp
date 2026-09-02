// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMGameMode.h"

#include "TDMGameState.h"
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

	if (!GetGameState<ATDMGameState>()->HasMatchEnded()) {
		ATDMPlayerState* const victimPlayerState = aVictimController->GetPlayerState<ATDMPlayerState>();
		victimPlayerState->AddDeath();
		if (aKillerController) {
			ATDMPlayerState* const killerPlayerState = aKillerController->GetPlayerState<ATDMPlayerState>();
			if (victimPlayerState != killerPlayerState && victimPlayerState->GetTeamId() != killerPlayerState->GetTeamId()) {
				killerPlayerState->AddKill();
				const int32 teamKills = GetGameState<ATDMGameState>()->AddTeamKill(killerPlayerState->GetTeamId());
				if (teamKills >= myScoreLimit) {
					FinishMatch(killerPlayerState->GetTeamId());
				}
			}
		}
	}
}

void ATDMGameMode::FinishMatch(const EValkTeamId aTeamId)
{
	ATDMGameState* tdmGameState = GetGameState<ATDMGameState>();
	if (!tdmGameState->HasMatchEnded()) {
		tdmGameState->SetMatchEnded(aTeamId);
		GetWorldTimerManager().SetTimer(
			myReturnToMainMenuTimerHandle,
			this,
			&ATDMGameMode::ReturnPlayersToMainMenu,
			myPostMatchDelay,
			false
		);
	}
}

void ATDMGameMode::ReturnPlayersToMainMenu() const
{
	GetWorld()->ServerTravel(TEXT("/Game/Maps/Level_MainMenu"));
}
