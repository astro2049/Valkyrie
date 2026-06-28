// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/UI/UIValkHUD.h"
#include "UIExtractionHUD.generated.h"

class UTextBlock;

UCLASS(Blueprintable)
class VALKYRIE_API UUIExtractionHUD : public UUIValkHUD
{
	GENERATED_BODY()

protected:
	virtual void UpdateModeFromViewModel() const override;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myObjectiveText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myDefenseTimerText{nullptr};
};
