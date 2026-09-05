// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomGameState.h"

#include "Net/UnrealNetwork.h"

void ARoomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoomGameState, myMap);
}

void ARoomGameState::SetMap(UDataAsset_Map* const aMapDataAsset)
{
	myMap = aMapDataAsset;
	myMapChanged.Broadcast();
}

void ARoomGameState::OnRep_Map()
{
	myMapChanged.Broadcast();
}
