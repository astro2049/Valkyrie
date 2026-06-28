// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PVPScoreboardViewModel.h"
#include "UIPVPScoreboard.generated.h"

class UUIPVPScoreboardRow;
class UTextBlock;
class UVerticalBox;

UCLASS(Blueprintable)
class VALKYRIE_API UUIPVPScoreboard : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetViewModel(UPVPScoreboardViewModel* aViewModel);
	void RebuildRows();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUIPVPScoreboardRow> myRowWidgetClass;
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myTeamAScoreText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myTeamBScoreText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UVerticalBox> myTeamARows{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UVerticalBox> myTeamBRows{nullptr};

private:
	void HandleViewModelChanged();
	void AddRows(UVerticalBox* aContainer, const TArray<FPVPScoreboardRowData>& aRows) const;

	UPROPERTY()
	TObjectPtr<UPVPScoreboardViewModel> myViewModel{nullptr};
};
