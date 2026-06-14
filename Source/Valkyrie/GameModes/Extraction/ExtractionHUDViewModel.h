// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/UI/ValkHUDViewModel.h"
#include "ExtractionHUDViewModel.generated.h"

class AExtractionGameState;

UCLASS()
class VALKYRIE_API UExtractionHUDViewModel : public UValkHUDViewModel
{
	GENERATED_BODY()

public:
	void BindToExtractionGameState(AExtractionGameState* aGameState);
	void UnbindExtractionGameState();
	void RefreshFromExtractionGameState();

	FText GetObjectiveText() const { return myObjectiveText; }
	float GetDefenseTimeRemaining() const { return myDefenseTimeRemaining; }
	bool ShouldShowDefenseTimer() const { return myShowDefenseTimer; }

private:
	UFUNCTION()
	void HandleObjectiveChanged(FText anObjectiveText);
	UFUNCTION()
	void HandleDefenseTimerChanged(float aDefenseTimeRemaining, bool aShowDefenseTimer);

	UPROPERTY()
	TObjectPtr<AExtractionGameState> myExtractionGameState{nullptr};

	FText myObjectiveText;
	float myDefenseTimeRemaining{0.f};
	bool myShowDefenseTimer{false};
};
