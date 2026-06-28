// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIValkHUD.generated.h"

class UProgressBar;
class UTextBlock;
class UValkHUDViewModel;
class UWidget;

UCLASS(Abstract, Blueprintable)
class VALKYRIE_API UUIValkHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetViewModel(UValkHUDViewModel* aViewModel);

protected:
	virtual void UpdateModeFromViewModel() const PURE_VIRTUAL(UUIValkHUD::UpdateModeFromViewModel,);
	UValkHUDViewModel* GetViewModel() const { return myViewModel; }

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
	TObjectPtr<UWidget> myInteractPrompt{nullptr};

private:
	void UpdateFromViewModel() const;
	void HandleViewModelChanged();

	UPROPERTY()
	TObjectPtr<UValkHUDViewModel> myViewModel{nullptr};
};
