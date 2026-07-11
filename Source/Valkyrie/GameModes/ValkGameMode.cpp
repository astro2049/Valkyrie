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
	if (aNewPlayer) {
		if (AValkPlayerState* const playerState = aNewPlayer->GetPlayerState<AValkPlayerState>()) {
			if (playerState->GetTeamId() == EValkTeamId::None) {
				playerState->SetTeamId(EValkTeamId::TeamA);
			}
		}
	}

	Super::PostLogin(aNewPlayer);
}

AActor* AValkGameMode::ChoosePlayerStart_Implementation(AController* const aPlayer)
{
	if (!aPlayer) {
		return Super::ChoosePlayerStart_Implementation(aPlayer);
	}

	AActor* selectedPlayerStart = nullptr;
	if (const AValkPlayerState* const playerState = aPlayer->GetPlayerState<AValkPlayerState>()) {
		FName playerStartTag;
		switch (playerState->GetTeamId()) {
		case EValkTeamId::TeamA:
			playerStartTag = FName(TEXT("TeamA"));
			break;
		case EValkTeamId::TeamB:
			playerStartTag = FName(TEXT("TeamB"));
			break;
		default:
			break;
		}

		if (!playerStartTag.IsNone()) {
			TArray<AActor*> playerStartActors;
			UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), playerStartActors);
			TArray<APlayerStart*> teamPlayerStarts;
			for (AActor* const playerStartActor : playerStartActors) {
				if (APlayerStart* const playerStart = Cast<APlayerStart>(playerStartActor);
					playerStart && playerStart->PlayerStartTag == playerStartTag) {
					teamPlayerStarts.Add(playerStart);
				}
			}

			if (!teamPlayerStarts.IsEmpty()) {
				teamPlayerStarts.Sort([](const APlayerStart& aLeft, const APlayerStart& aRight) {
					return aLeft.GetName() < aRight.GetName();
				});
				int32 playerIndex = 0;
				if (const UWorld* const world = GetWorld()) {
					for (FConstPlayerControllerIterator iterator = world->GetPlayerControllerIterator(); iterator; ++iterator) {
						if (const APlayerController* const playerController = iterator->Get()) {
							if (const AValkPlayerState* const currentPlayerState = playerController->GetPlayerState<AValkPlayerState>()) {
								if (currentPlayerState->GetTeamId() == playerState->GetTeamId()) {
									if (playerController == aPlayer) {
										break;
									}
									++playerIndex;
								}
							}
						}
					}
				}
				selectedPlayerStart = teamPlayerStarts[playerIndex % teamPlayerStarts.Num()];
			}
		}
	}

	if (!selectedPlayerStart) {
		selectedPlayerStart = Super::ChoosePlayerStart_Implementation(aPlayer);
	}
	return selectedPlayerStart;
}

void AValkGameMode::ScheduleReturnToMainMenu()
{
	if (!GetWorldTimerManager().IsTimerActive(myReturnTimerHandle)) {
		GetWorldTimerManager().SetTimer(
			myReturnTimerHandle,
			this,
			&AValkGameMode::ReturnPlayersToMainMenu,
			myPostMatchDelay,
			false
		);
	}
}

void AValkGameMode::OnPlayerDied(AController* const, AController* const aVictimController)
{
	if (AValkPlayerController* const playerController = Cast<AValkPlayerController>(aVictimController)) {
		playerController->Client_OnPlayerDied();
	}
}

void AValkGameMode::ReturnPlayersToMainMenu() const
{
	if (UWorld* const world = GetWorld()) {
		world->ServerTravel(TEXT("/Game/MainMenu/MainMenu"));
	}
}
