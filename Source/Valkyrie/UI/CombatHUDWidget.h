// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatHUDWidget.generated.h"

class UPlayerCombatViewModel;
class UTextBlock;
class UWidget;

UCLASS(BlueprintType, Blueprintable)
class VALKYRIE_API UCombatHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Combat HUD")
	void SetViewModel(UPlayerCombatViewModel* aViewModel);

	UFUNCTION(BlueprintCallable, Category="Combat HUD")
	void UpdateFromViewModel();

protected:
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional), Category="Combat HUD|Ammo")
	TObjectPtr<UTextBlock> myAmmoText{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional), Category="Combat HUD|Ammo")
	TObjectPtr<UTextBlock> myReserveAmmoText{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional), Category="Combat HUD|Ammo")
	TObjectPtr<UWidget> myReloadingIndicator{nullptr};

private:
	UFUNCTION()
	void HandleViewModelChanged();

	UPROPERTY()
	TObjectPtr<UPlayerCombatViewModel> myViewModel{nullptr};
};
