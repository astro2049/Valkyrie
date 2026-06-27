// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/UI/ValkHUDViewModel.h"
#include "ExtractionHUDViewModel.generated.h"

class AExtractionGameState;

struct FExtractionHUDData
{
	FText myObjectiveText;
	float myDefenseTimeRemaining{0.f};
	bool myShowDefenseTimer{false};
};

UCLASS()
class VALKYRIE_API UExtractionHUDViewModel : public UValkHUDViewModel
{
	GENERATED_BODY()

public:
	void BindToGameState(AExtractionGameState* aGameState);
	void RefreshFromExtractionGameState();

	const FExtractionHUDData& GetExtractionHUDData() const { return myExtractionHUDData; }

private:
	void HandleExtractionStateChanged();

	TWeakObjectPtr<AExtractionGameState> myExtractionGameState;

	FExtractionHUDData myExtractionHUDData;
};
