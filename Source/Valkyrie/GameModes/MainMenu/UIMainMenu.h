// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapListPanel//MapDataAsset.h"
#include "UIMainMenu.generated.h"

class UUIMainMenuPanel;
class UUIMapListPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMenuScreenQuitRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMenuScreenStartRequested, UMapDataAsset*, aMapData, FMenuModeEntry, aModeEntry);

UCLASS(BlueprintType, Blueprintable)
class VALKYRIE_API UUIMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void ShowMainMenu();
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void ShowMapSelect();

	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FMenuScreenQuitRequested myOnQuitRequested;
	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FMenuScreenStartRequested myOnStartRequested;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UUIMainMenuPanel> myMainMenuPanel{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UUIMapListPanel> myMapListPanel{nullptr};

private:
	UFUNCTION()
	void HandleSelectMapRequested();
	UFUNCTION()
	void HandleQuitRequested();
	UFUNCTION()
	void HandleBackRequested();
	UFUNCTION()
	void HandleStartRequested(UMapDataAsset* aMapData, FMenuModeEntry aModeEntry);

	void BindChildWidgets();
	void UnbindChildWidgets();
};
