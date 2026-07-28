// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UINode_GetMaxPlayersPerTeam.generated.h"

UCLASS()
class VALKYRIE_API UUINode_GetMaxPlayersPerTeam : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	static int32 GetMaxPlayersPerTeam();
};
