// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetRoomData.h"

#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"

void UUINode_GetRoomData::GetRoomData(APlayerController* aPlayerController,
                                      TArray<FString>& someTeamAPlayerNames,
                                      TArray<FString>& someTeamBPlayerNames)
{
	someTeamAPlayerNames.Reset();
	someTeamBPlayerNames.Reset();
	someTeamAPlayerNames.Reserve(ValkGameRules::MaxPlayersPerTeam);
	someTeamBPlayerNames.Reserve(ValkGameRules::MaxPlayersPerTeam);
	if (aPlayerController) {
		if (const UWorld* const world = aPlayerController->GetWorld()) {
			if (const AGameStateBase* const gameState = world->GetGameState<AGameStateBase>()) {
				for (const APlayerState* const playerState : gameState->PlayerArray) {
					if (const AValkPlayerState* const valkPlayerState = Cast<AValkPlayerState>(playerState)) {
						if (valkPlayerState->GetTeamId() == EValkTeamId::TeamA
							&& someTeamAPlayerNames.Num() < ValkGameRules::MaxPlayersPerTeam) {
							someTeamAPlayerNames.Add(valkPlayerState->GetPlayerName());
						} else if (valkPlayerState->GetTeamId() == EValkTeamId::TeamB
							&& someTeamBPlayerNames.Num() < ValkGameRules::MaxPlayersPerTeam) {
							someTeamBPlayerNames.Add(valkPlayerState->GetPlayerName());
						}
					}
				}
			}
		}
	}
}
