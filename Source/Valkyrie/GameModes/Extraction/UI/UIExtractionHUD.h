// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIExtractionHUD.generated.h"

class UExtractionHUDViewModel;
class UTextBlock;

UCLASS(Blueprintable)
class VALKYRIE_API UUIExtractionHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& aGeometry, float aDeltaSeconds) override;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myObjectiveText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myDefenseTimerText{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UExtractionHUDViewModel> myViewModelClass;

private:
	void RefreshModeData();
	void UpdateFromViewModel() const;

	UPROPERTY()
	TObjectPtr<UExtractionHUDViewModel> myViewModel{nullptr};
};
