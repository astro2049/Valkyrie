// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapDataAsset.h"
#include "UIMapListPanel.generated.h"

class UImage;
class UUIButton;
class UUIMapItem;
class UUIGameModeItem;
class UPanelWidget;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMapSelectBackRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMapSelectStartRequested, UMapDataAsset*, aMapData, FMenuModeEntry, aModeEntry);

UCLASS(BlueprintType, Blueprintable)
class VALKYRIE_API UUIMapListPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void ResetMapSelection();

	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FMapSelectBackRequested myOnBackRequested;
	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FMapSelectStartRequested myOnStartRequested;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valkyrie")
	TArray<TObjectPtr<UMapDataAsset>> myAvailableMaps;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valkyrie")
	TSubclassOf<UUIMapItem> myMapEntryWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valkyrie")
	TSubclassOf<UUIGameModeItem> myModeEntryWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UUIButton> myBackButton{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UUIButton> myStartButton{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UPanelWidget> myMapListContainer{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UPanelWidget> myModeListContainer{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UImage> myPreviewImage{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UTextBlock> myMapNameText{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UTextBlock> myMapDescriptionText{nullptr};

private:
	UFUNCTION()
	void HandleBackClicked();
	UFUNCTION()
	void HandleStartClicked();
	UFUNCTION()
	void HandleMapEntryHovered(int32 anEntryIndex);
	UFUNCTION()
	void HandleMapEntryClicked(int32 anEntryIndex);
	UFUNCTION()
	void HandleModeEntryClicked(int32 anEntryIndex);

	void BindButtons();
	void UnbindButtons();
	void BuildMapList();
	void BuildModeList(const UMapDataAsset* aMapData);
	void ClearMapList();
	void ClearModeList();
	void PreviewMap(int32 anEntryIndex);
	void SelectMap(int32 anEntryIndex);
	void SelectMode(int32 anEntryIndex);
	void RefreshMapDetails(const UMapDataAsset* aMapData);
	void RefreshMapEntrySelection();
	void RefreshModeEntrySelection();
	void RefreshStartState();
	int32 FindFirstPlayableModeIndex(const UMapDataAsset* aMapData) const;
	bool IsValidMapIndex(int32 anEntryIndex) const;
	bool IsValidModeIndex(const UMapDataAsset* aMapData, int32 anEntryIndex) const;

	UPROPERTY()
	TArray<TObjectPtr<UUIMapItem>> myMapEntryWidgets;
	UPROPERTY()
	TArray<TObjectPtr<UUIGameModeItem>> myModeEntryWidgets;

	int32 myPreviewedMapIndex{INDEX_NONE};
	int32 mySelectedMapIndex{INDEX_NONE};
	int32 mySelectedModeIndex{INDEX_NONE};
};
