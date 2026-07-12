// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UINode_GetTDMScoreboardData.generated.h"

class APlayerController;

UCLASS()
class VALKYRIE_API UUINode_GetTDMScoreboardData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static void GetTDMScoreboardData(
		APlayerController* aPlayerController,
		int32& aTeamAScore,
		int32& aTeamBScore,
		TArray<int32>& someTeamAPlayerIds,
		TArray<int32>& someTeamBPlayerIds
	);
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static void GetTDMScoreboardRowData(
		APlayerController* aPlayerController,
		int32 aPlayerId,
		FString& aPlayerName,
		int32& aKills,
		int32& aDeaths
	);
};
