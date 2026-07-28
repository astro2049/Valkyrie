// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UINode_GetTDMScoreboardData.generated.h"

class APlayerController;

USTRUCT(BlueprintType)
struct FValkTDMScoreboardRowData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	FString myPlayerName;
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myKills{-1};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myDeaths{-1};
};

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
		TArray<FValkTDMScoreboardRowData>& someTeamARows,
		TArray<FValkTDMScoreboardRowData>& someTeamBRows
	);
};
