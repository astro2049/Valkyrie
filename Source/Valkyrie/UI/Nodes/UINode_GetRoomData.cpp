// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetRoomData.h"

#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"

void UUINode_GetRoomData::GetRoomData(APlayerController* aPlayerController,
                                      TArray<int32>& outTeamAPlayerIds,
                                      TArray<int32>& outTeamBPlayerIds)
{
	outTeamAPlayerIds.Init(-1, ValkGameRules::MaxPlayersPerTeam);
	outTeamBPlayerIds.Init(-1, ValkGameRules::MaxPlayersPerTeam);
	int32 teamAPlayerIndex = 0;
	int32 teamBPlayerIndex = 0;
	if (aPlayerController) {
		if (const UWorld* const world = aPlayerController->GetWorld()) {
			if (const AGameStateBase* const gameState = world->GetGameState<AGameStateBase>()) {
				for (const APlayerState* const playerState : gameState->PlayerArray) {
					if (playerState) {
						if (const AValkPlayerState* const valkPlayerState = Cast<AValkPlayerState>(playerState)) {
							if (valkPlayerState->GetTeamId() == EValkTeamId::TeamA
								&& teamAPlayerIndex < ValkGameRules::MaxPlayersPerTeam) {
								outTeamAPlayerIds[teamAPlayerIndex++] = valkPlayerState->GetPlayerId();
							} else if (valkPlayerState->GetTeamId() == EValkTeamId::TeamB
								&& teamBPlayerIndex < ValkGameRules::MaxPlayersPerTeam) {
								outTeamBPlayerIds[teamBPlayerIndex++] = valkPlayerState->GetPlayerId();
							}
						}
					}
				}
			}
		}
	}
}
