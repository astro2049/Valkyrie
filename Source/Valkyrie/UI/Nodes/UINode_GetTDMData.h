// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "UINode_GetTDMData.generated.h"

class APlayerController;

USTRUCT(BlueprintType)
struct FValkTDMData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myTeamAScore{0};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myTeamBScore{0};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	bool myMatchEnded{false};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	EValkTeamId myWinningTeamId{EValkTeamId::None};
};

UCLASS()
class VALKYRIE_API UUINode_GetTDMData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static FValkTDMData GetTDMData(APlayerController* aPlayerController);
};
