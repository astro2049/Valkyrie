// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIPVPScoreboardRow.generated.h"

class UTextBlock;
struct FPVPScoreboardRowData;

UCLASS(Blueprintable)
class VALKYRIE_API UUIPVPScoreboardRow : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetData(const FPVPScoreboardRowData& aRowData, bool aShowConfirms) const;

protected:
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myPlayerNameText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myConfirmsText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myKillsText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myDeathsText{nullptr};
};
