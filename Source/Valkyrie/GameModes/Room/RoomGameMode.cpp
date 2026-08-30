// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomGameMode.h"

#include "Valkyrie/GameModes/MainMenu/MapDataAsset.h"
#include "Valkyrie/GameModes/MainMenu/ValkGameInstance.h"

ARoomGameMode::ARoomGameMode()
{
	bUseSeamlessTravel = true;
}

void ARoomGameMode::InitGame(
	const FString& aMapName,
	const FString& someOptions,
	FString& anErrorMessage
)
{
	Super::InitGame(aMapName, someOptions, anErrorMessage);

	const UValkGameInstance* const gameInstance = GetGameInstance<UValkGameInstance>();
	if (const UMapDataAsset* const mapDataAsset = gameInstance->GetSelectedMapDataAsset()) {
		myTeamCount = mapDataAsset->myTeamCount;
	}
}

void ARoomGameMode::StartGame(const TSoftObjectPtr<UWorld> aLevel) const
{
	GetWorld()->ServerTravel(aLevel.ToSoftObjectPath().GetLongPackageName());
}
