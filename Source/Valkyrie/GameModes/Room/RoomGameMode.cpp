// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "Valkyrie/Common/ValkTeamAssignment.h"
#include "Valkyrie/GameModes/MainMenu/MapDataAsset.h"
#include "Valkyrie/GameModes/MainMenu/ValkGameInstance.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"

ARoomGameMode::ARoomGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	PlayerStateClass = AValkPlayerState::StaticClass();
	bUseSeamlessTravel = true;
}

void ARoomGameMode::PostLogin(APlayerController* const aNewPlayer)
{
	const UMapDataAsset* const mapDataAsset = GetGameInstance<UValkGameInstance>()->GetSelectedMapDataAsset();
	AValkPlayerState* const playerState = aNewPlayer->GetPlayerState<AValkPlayerState>();
	ValkTeamAssignment::AssignTeam(*GetGameState<AGameStateBase>(), *playerState, mapDataAsset->myTeamCount);

	Super::PostLogin(aNewPlayer);
}

void ARoomGameMode::StartGame(const TSoftObjectPtr<UWorld> aLevel) const
{
	GetWorld()->ServerTravel(aLevel.ToSoftObjectPath().GetLongPackageName());
}
