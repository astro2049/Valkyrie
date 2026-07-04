// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UINode_GetPVPScoreboardData.generated.h"

class APlayerController;

USTRUCT(BlueprintType)
struct FValkScoreboardRowData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	FString myPlayerName;
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myKills{0};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myDeaths{0};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myConfirms{0};
};

UCLASS()
class VALKYRIE_API UUINode_GetPVPScoreboardData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static void GetPVPScoreboardData(
		APlayerController* aPlayerController,
		int32& aTeamAScore,
		int32& aTeamBScore,
		bool& aShowConfirms,
		TArray<FValkScoreboardRowData>& someTeamARows,
		TArray<FValkScoreboardRowData>& someTeamBRows
	);
};
