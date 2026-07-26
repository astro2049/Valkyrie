// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UINode_GetRoomData.generated.h"

class APlayerController;

UCLASS()
class VALKYRIE_API UUINode_GetRoomData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static void GetRoomData(APlayerController* aPlayerController,
	                        TArray<int32>& outTeamAPlayerIds,
	                        TArray<int32>& outTeamBPlayerIds);
};
