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
	void RefreshHUDData();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& aGeometry, float aDeltaSeconds) override;
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
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UValkHUDViewModel> myViewModelClass;

private:
	void UpdateFromViewModel() const;

	UPROPERTY()
	TObjectPtr<UValkHUDViewModel> myViewModel{nullptr};
};
