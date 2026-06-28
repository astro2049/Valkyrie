// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ValkPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FValkPlayerStateChanged);

UCLASS()
class VALKYRIE_API AValkPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	FValkPlayerStateChanged myOnPlayerStateChanged; // for player list and scoreboard view models
};
