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
	virtual void RefreshData() override;

	const FExtractionHUDData& GetExtractionHUDData() const { return myExtractionHUDData; }

private:
	void RefreshExtractionHUDData();

	FExtractionHUDData myExtractionHUDData;
};
