// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIExtractionHUD.generated.h"

class UExtractionHUDViewModel;
class UProgressBar;
class UTextBlock;
class UWidget;

UCLASS(Blueprintable)
class VALKYRIE_API UUIExtractionHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetViewModel(UExtractionHUDViewModel* aViewModel);
	void UpdateFromViewModel() const;

protected:
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myAmmoText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myReserveAmmoText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UWidget> myReloadingIndicator{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UProgressBar> myHealthBar{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myHealthText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myObjectiveText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myDefenseTimerText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myInteractPrompt{nullptr};

private:
	void HandleViewModelChanged();

	UPROPERTY()
	TObjectPtr<UExtractionHUDViewModel> myViewModel{nullptr};
};
