// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomGameMode.h"

#include "RoomGameState.h"
#include "Valkyrie/Common/ValkTeamAssignment.h"
#include "Valkyrie/Data/DataAsset_Map.h"
#include "Valkyrie/GameModes/MainMenu/ValkGameInstance.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"

ARoomGameMode::ARoomGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	GameStateClass = ARoomGameState::StaticClass();
	PlayerStateClass = AValkPlayerState::StaticClass();
	bUseSeamlessTravel = true;
}

void ARoomGameMode::InitGameState()
{
	Super::InitGameState();

	GetGameState<ARoomGameState>()->SetMap(GetGameInstance<UValkGameInstance>()->GetSelectedMapDataAsset());
}

void ARoomGameMode::PostLogin(APlayerController* const aNewPlayer)
{
	AValkPlayerState* const playerState = aNewPlayer->GetPlayerState<AValkPlayerState>();
	const UDataAsset_Map* const mapDataAsset = GetGameState<ARoomGameState>()->GetMap();
	ValkTeamAssignment::AssignTeam(*GetGameState<ARoomGameState>(), *playerState, mapDataAsset->myTeamCount);

	Super::PostLogin(aNewPlayer);
}

void ARoomGameMode::StartGame() const
{
	const UDataAsset_Map* const mapDataAsset = GetGameState<ARoomGameState>()->GetMap();
	GetWorld()->ServerTravel(mapDataAsset->myLevel.ToSoftObjectPath().GetLongPackageName());
}
