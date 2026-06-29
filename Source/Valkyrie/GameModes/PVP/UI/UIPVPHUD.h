// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIPVPHUD.generated.h"

class UPVPHUDViewModel;
class UTextBlock;

UCLASS(Blueprintable)
class VALKYRIE_API UUIPVPHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& aGeometry, float aDeltaSeconds) override;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myTeamAScoreText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myTeamBScoreText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myWinnerText{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UPVPHUDViewModel> myViewModelClass;

private:
	void RefreshModeData();
	void UpdateFromViewModel() const;

	UPROPERTY()
	TObjectPtr<UPVPHUDViewModel> myViewModel{nullptr};
};
