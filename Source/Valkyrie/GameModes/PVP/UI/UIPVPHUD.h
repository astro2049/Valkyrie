// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIPVPHUD.generated.h"

class UPVPHUDViewModel;
class UProgressBar;
class UTextBlock;
class UWidget;

UCLASS(Blueprintable)
class VALKYRIE_API UUIPVPHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetViewModel(UPVPHUDViewModel* aViewModel);
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
	TObjectPtr<UTextBlock> myTeamAScoreText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myTeamBScoreText{nullptr};
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> myWinnerText{nullptr};

private:
	void HandleViewModelChanged();

	UPROPERTY()
	TObjectPtr<UPVPHUDViewModel> myViewModel{nullptr};
};
