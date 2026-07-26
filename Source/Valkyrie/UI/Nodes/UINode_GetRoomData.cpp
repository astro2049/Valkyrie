// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetRoomData.h"

#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"

void UUINode_GetRoomData::GetRoomData(APlayerController* aPlayerController,
                                      TArray<int32>& outTeamAPlayerIds,
                                      TArray<int32>& outTeamBPlayerIds)
{
	outTeamAPlayerIds.Reset();
	outTeamBPlayerIds.Reset();
	if (aPlayerController) {
		if (const UWorld* const world = aPlayerController->GetWorld()) {
			if (const AGameStateBase* const gameState = world->GetGameState<AGameStateBase>()) {
				for (const APlayerState* const playerState : gameState->PlayerArray) {
					if (playerState) {
						if (const AValkPlayerState* const valkPlayerState = Cast<AValkPlayerState>(playerState)) {
							if (valkPlayerState->GetTeamId() == EValkTeamId::TeamA) {
								outTeamAPlayerIds.Add(valkPlayerState->GetPlayerId());
							} else if (valkPlayerState->GetTeamId() == EValkTeamId::TeamB) {
								outTeamBPlayerIds.Add(valkPlayerState->GetPlayerId());
							}
						}
					}
				}
			}
		}
	}
}
