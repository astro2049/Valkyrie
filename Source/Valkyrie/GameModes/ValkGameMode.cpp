// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "ValkGameState.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"

AValkGameMode::AValkGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	PlayerStateClass = AValkPlayerState::StaticClass();
}

void AValkGameMode::PostLogin(APlayerController* const aNewPlayer)
{
	Super::PostLogin(aNewPlayer);

	if (!aNewPlayer) {
		return;
	}

	if (AValkPlayerState* const playerState = aNewPlayer->GetPlayerState<AValkPlayerState>()) {
		if (playerState->GetTeamId() == EValkTeamId::None) {
			AssignTeam(*playerState);
		}
	}
}

void AValkGameMode::AssignTeam(AValkPlayerState& aPlayerState) const
{
	if (aPlayerState.GetTeamId() != EValkTeamId::None) {
		return;
	}

	if (myTeamCount == 1) {
		aPlayerState.SetTeamId(EValkTeamId::TeamA);
		return;
	}

	int32 teamAPlayerCount = 0;
	int32 teamBPlayerCount = 0;
	if (const AGameStateBase* const gameState = GetGameState<AGameStateBase>()) {
		for (const APlayerState* const playerState : gameState->PlayerArray) {
			if (const AValkPlayerState* const valkPlayerState = Cast<AValkPlayerState>(playerState)) {
				if (valkPlayerState->GetTeamId() == EValkTeamId::TeamA) {
					teamAPlayerCount++;
				} else if (valkPlayerState->GetTeamId() == EValkTeamId::TeamB) {
					teamBPlayerCount++;
				}
			}
		}
	}
	aPlayerState.SetTeamId(teamAPlayerCount <= teamBPlayerCount ? EValkTeamId::TeamA : EValkTeamId::TeamB);
}

AActor* AValkGameMode::ChoosePlayerStart_Implementation(AController* const aPlayer)
{
	if (!aPlayer) {
		return Super::ChoosePlayerStart_Implementation(aPlayer);
	}

	static const TMap<EValkTeamId, FString> teamNameMap = {
		{EValkTeamId::None, "None"},
		{EValkTeamId::TeamA, "TeamA"},
		{EValkTeamId::TeamB, "TeamB"}
	};

	// calculate index in team
	if (const AValkPlayerState* const playerState = aPlayer->GetPlayerState<AValkPlayerState>()) {
		int32 indexInTeam = 0;
		if (const AGameStateBase* const gameState = GetGameState<AGameStateBase>()) {
			for (const APlayerState* const currentPlayerState : gameState->PlayerArray) {
				if (currentPlayerState == playerState) {
					break;
				}

				if (const AValkPlayerState* const currentValkPlayerState = Cast<AValkPlayerState>(currentPlayerState);
					currentValkPlayerState && currentValkPlayerState->GetTeamId() == playerState->GetTeamId()) {
					indexInTeam++;
				}
			}
		}

		TArray<AActor*> playerStartActors;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), playerStartActors);
		for (AActor* const playerStartActor : playerStartActors) {
			if (APlayerStart* const playerStart = Cast<APlayerStart>(playerStartActor)) {
				if (playerStart->PlayerStartTag.IsEqual(
					// so PlayerStartTag should be like TeamA_0, TeamA_1...
					FName(teamNameMap[playerState->GetTeamId()] + "_" + FString::FromInt(indexInTeam))
				)) {
					return playerStart;
				}
			}
		}
	}

	return Super::ChoosePlayerStart_Implementation(aPlayer);
}

void AValkGameMode::PlayerDied(AController* const, AController* const aVictimController)
{
	if (AValkPlayerController* const playerController = Cast<AValkPlayerController>(aVictimController)) {
		playerController->Client_OnPlayerDied();
	}
}

void AValkGameMode::FinishMatch()
{
	if (AValkGameState* const gameState = GetGameState<AValkGameState>()) {
		if (!gameState->HasMatchEnded()) {
			gameState->SetMatchEnded();
			ReturnToMainMenuAfterDelay();
		}
	}
}

void AValkGameMode::ReturnToMainMenuAfterDelay()
{
	if (!GetWorldTimerManager().IsTimerActive(myReturnToMainMenuTimerHandle)) {
		GetWorldTimerManager().SetTimer(
			myReturnToMainMenuTimerHandle,
			this,
			&AValkGameMode::ReturnPlayersToMainMenu,
			myPostMatchDelay,
			false
		);
	}
}

void AValkGameMode::ReturnPlayersToMainMenu() const
{
	if (UWorld* const world = GetWorld()) {
		world->ServerTravel(TEXT("/Game/Maps/Level_MainMenu"));
	}
}
