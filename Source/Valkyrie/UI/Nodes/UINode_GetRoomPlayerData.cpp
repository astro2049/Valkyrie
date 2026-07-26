// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetRoomPlayerData.h"

#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

void UUINode_GetRoomPlayerData::GetRoomPlayerData(
	APlayerController* const aPlayerController,
	const int32 aPlayerId,
	FString& aPlayerName
)
{
	aPlayerName = FString();
	if (aPlayerController) {
		if (const UWorld* const world = aPlayerController->GetWorld()) {
			if (const AGameStateBase* const gameState = world->GetGameState<AGameStateBase>()) {
				for (const APlayerState* const playerState : gameState->PlayerArray) {
					if (playerState && playerState->GetPlayerId() == aPlayerId) {
						aPlayerName = playerState->GetPlayerName();
						break;
					}
				}
			}
		}
	}
}
