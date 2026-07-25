// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
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

	// assign team id
	if (AValkPlayerState* const playerState = aNewPlayer->GetPlayerState<AValkPlayerState>()) {
		if (playerState->GetTeamId() == EValkTeamId::None) {
			playerState->SetTeamId(EValkTeamId::TeamA);
		}

		// assign index in team
		const EValkTeamId teamId = playerState->GetTeamId();
		if (teamId == EValkTeamId::TeamA) {
			playerState->SetIndexInTeam(myTeamAPlayerCount++);
		} else if (teamId == EValkTeamId::TeamB) {
			playerState->SetIndexInTeam(myTeamBPlayerCount++);
		}
	}
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

	TArray<AActor*> playerStartActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), playerStartActors);
	for (AActor* const playerStartActor : playerStartActors) {
		APlayerStart* const playerStart = Cast<APlayerStart>(playerStartActor);
		const AValkPlayerState* const playerState = aPlayer->GetPlayerState<AValkPlayerState>();
		if (playerStart && playerState) {
			FString playerStartString = teamNameMap[playerState->GetTeamId()] + "_" + FString::FromInt(playerState->GetIndexInTeam());
			// so PlayerStartTag should be like TeamA_0, TeamA_1...
			if (playerStart->PlayerStartTag.IsEqual(FName(playerStartString))) {
				return playerStart;
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
