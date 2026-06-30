// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Valkyrie/GameModes/Extraction/ExtractionTypes.h"
#include "UIDataExtractionLibrary.generated.h"

class APlayerController;

USTRUCT(BlueprintType)
struct FValkExtractionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	ECombatSliceState myCombatSliceState{ECombatSliceState::ToStartGenerator};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	float myDefenseTimeRemaining{0.f};

	bool operator==(const FValkExtractionData& anOther) const
	{
		return myCombatSliceState == anOther.myCombatSliceState &&
			myDefenseTimeRemaining == anOther.myDefenseTimeRemaining;
	}
};

UCLASS()
class VALKYRIE_API UUIDataExtractionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static FValkExtractionData GetExtractionData(APlayerController* aPlayerController);
	UFUNCTION(BlueprintPure, Category="Valkyrie", meta=(DisplayName="Equal (Valk Extraction HUD Data)", CompactNodeTitle="=="))
	static bool EqualEqual_ValkExtractionData(
		const FValkExtractionData& aLeft,
		const FValkExtractionData& aRight
	);
};
