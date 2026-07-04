// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Valkyrie/GameModes/Extraction/ExtractionTypes.h"
#include "UINode_GetExtractionData.generated.h"

class APlayerController;

USTRUCT(BlueprintType)
struct FValkExtractionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	ECombatSliceState myCombatSliceState{ECombatSliceState::ToStartGenerator};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	float myDefenseTimeRemaining{0.f};
};

UCLASS()
class VALKYRIE_API UUINode_GetExtractionData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static FValkExtractionData GetExtractionData(APlayerController* aPlayerController);
};
