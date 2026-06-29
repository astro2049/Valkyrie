// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/Extraction/ExtractionTypes.h"
#include "Valkyrie/UI/ValkViewModel.h"
#include "ExtractionHUDViewModel.generated.h"

class AExtractionGameState;

struct FExtractionHUDData
{
	ECombatSliceState myCombatSliceState{ECombatSliceState::ToStartGenerator};
	float myDefenseTimeRemaining{0.f};
};

UCLASS()
class VALKYRIE_API UExtractionHUDViewModel : public UValkViewModel
{
	GENERATED_BODY()

public:
	void RefreshData();

	const FExtractionHUDData& GetExtractionHUDData() const { return myExtractionHUDData; }

private:
	void RefreshExtractionHUDData();

	FExtractionHUDData myExtractionHUDData;
};
