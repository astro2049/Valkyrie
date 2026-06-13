// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CombatSlicePlayerController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class VALKYRIE_API ACombatSlicePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
