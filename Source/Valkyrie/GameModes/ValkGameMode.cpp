// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkGameMode.h"

#include "GameFramework/Controller.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Valkyrie/Common/ValkTeamAssignment.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"

AValkGameMode::AValkGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	PlayerControllerClass = AValkPlayerController::StaticClass();
	PlayerStateClass = AValkPlayerState::StaticClass();
}

void AValkGameMode::HandleStartingNewPlayer_Implementation(APlayerController* const aNewPlayer)
{
	AValkPlayerState* const playerState = aNewPlayer->GetPlayerState<AValkPlayerState>();
	// Direct gameplay-map PIE fallback; normal sessions receive their team in RoomGameMode.
	ValkTeamAssignment::AssignTeam(*GetGameState<AGameStateBase>(), *playerState, myTeamCount);

	Super::HandleStartingNewPlayer_Implementation(aNewPlayer);
}

AActor* AValkGameMode::ChoosePlayerStart_Implementation(AController* const aPlayer)
{
	static const TMap<EValkTeamId, FString> teamNameMap = {
		{EValkTeamId::None, "None"},
		{EValkTeamId::TeamA, "TeamA"},
		{EValkTeamId::TeamB, "TeamB"}
	};

	const AValkPlayerState* const playerState = aPlayer->GetPlayerState<AValkPlayerState>();
	int32 indexInTeam = 0;
	for (const APlayerState* const currentPlayerState : GetGameState<AGameStateBase>()->PlayerArray) {
		if (currentPlayerState == playerState) {
			break;
		}
		if (playerState->GetTeamId() == CastChecked<AValkPlayerState>(currentPlayerState)->GetTeamId()) {
			indexInTeam++;
		}
	}

	TArray<AActor*> playerStartActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), playerStartActors);
	for (AActor* const playerStartActor : playerStartActors) {
		APlayerStart* const playerStart = CastChecked<APlayerStart>(playerStartActor);
		if (playerStart->PlayerStartTag.IsEqual(FName(teamNameMap[playerState->GetTeamId()] + "_" + FString::FromInt(indexInTeam)))) {
			return playerStart; // so PlayerStartTag should be like TeamA_0, TeamA_1...
		}
	}

	return Super::ChoosePlayerStart_Implementation(aPlayer);
}

void AValkGameMode::PlayerDied(AController* const, AController* const aVictimController)
{
	if (!GetGameState<AGameStateBase>()->HasMatchEnded()) {
		FTimerDelegate respawnDelegate;
		respawnDelegate.BindUObject(this, &AValkGameMode::RespawnPlayer, aVictimController);
		FTimerHandle respawnTimerHandle;
		GetWorldTimerManager().SetTimer(
			respawnTimerHandle,
			respawnDelegate,
			myRespawnDelay,
			false
		);
	}
}

void AValkGameMode::RespawnPlayer(AController* const aController)
{
	if (!GetGameState<AGameStateBase>()->HasMatchEnded()) {
		APawn* const oldPawn = aController->GetPawn();
		aController->UnPossess();
		oldPawn->Destroy();
		RestartPlayer(aController);
		CastChecked<AValkPlayerController>(aController)->Client_OnPlayerRespawned();
	}
}
