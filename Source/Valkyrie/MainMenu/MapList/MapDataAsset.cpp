// Fill out your copyright notice in the Description page of Project Settings.

#include "MapDataAsset.h"

#include "GameFramework/GameModeBase.h"

bool FMenuModeEntry::IsPlayable() const
{
	return myGameModeClass != nullptr;
}
