// Fill out your copyright notice in the Description page of Project Settings.

#include "UIMapListPanel.h"

#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Valkyrie/UI/UIButton.h"
#include "UIMapItem.h"
#include "UIGameModeItem.h"

void UUIMapListPanel::ResetMapSelection()
{
	mySelectedMapIndex = INDEX_NONE;
	mySelectedModeIndex = INDEX_NONE;

	if (myAvailableMaps.Num() > 0) {
		PreviewMap(0);
	} else {
		RefreshMapDetails(nullptr);
		ClearModeList();
	}

	RefreshMapEntrySelection();
	RefreshModeEntrySelection();
	RefreshStartState();
}

void UUIMapListPanel::NativeConstruct()
{
	Super::NativeConstruct();

	BindButtons();
	BuildMapList();
	ResetMapSelection();
}

void UUIMapListPanel::NativeDestruct()
{
	UnbindButtons();
	ClearMapList();
	ClearModeList();

	Super::NativeDestruct();
}

void UUIMapListPanel::HandleBackClicked()
{
	myOnBackRequested.Broadcast();
}

void UUIMapListPanel::HandleStartClicked()
{
	if (!IsValidMapIndex(mySelectedMapIndex)) {
		return;
	}

	UMapDataAsset* selectedMap = myAvailableMaps[mySelectedMapIndex];
	if (!IsValidModeIndex(selectedMap, mySelectedModeIndex)) {
		return;
	}

	const FMenuModeEntry selectedMode = selectedMap->mySupportedModes[mySelectedModeIndex];
	if (!selectedMode.IsPlayable()) {
		return;
	}

	myOnStartRequested.Broadcast(selectedMap, selectedMode);
}

void UUIMapListPanel::HandleMapEntryHovered(int32 anEntryIndex)
{
	PreviewMap(anEntryIndex);
}

void UUIMapListPanel::HandleMapEntryClicked(int32 anEntryIndex)
{
	SelectMap(anEntryIndex);
}

void UUIMapListPanel::HandleModeEntryClicked(int32 anEntryIndex)
{
	SelectMode(anEntryIndex);
}

void UUIMapListPanel::BindButtons()
{
	myBackButton->myOnClicked.AddUniqueDynamic(this, &UUIMapListPanel::HandleBackClicked);
	myStartButton->myOnClicked.AddUniqueDynamic(this, &UUIMapListPanel::HandleStartClicked);
}

void UUIMapListPanel::UnbindButtons()
{
	myBackButton->myOnClicked.RemoveDynamic(this, &UUIMapListPanel::HandleBackClicked);
	myStartButton->myOnClicked.RemoveDynamic(this, &UUIMapListPanel::HandleStartClicked);
}

void UUIMapListPanel::BuildMapList()
{
	ClearMapList();

	if (!myMapEntryWidgetClass) {
		return;
	}

	for (int32 mapIndex = 0; mapIndex < myAvailableMaps.Num(); ++mapIndex) {
		UMapDataAsset* mapData = myAvailableMaps[mapIndex];
		if (!mapData) {
			continue;
		}

		UUIMapItem* mapEntry = CreateWidget<UUIMapItem>(GetOwningPlayer(), myMapEntryWidgetClass);
		if (!mapEntry) {
			continue;
		}

		mapEntry->InitializeMapEntry(mapIndex, mapData->myDisplayName);
		mapEntry->myOnHovered.AddUniqueDynamic(this, &UUIMapListPanel::HandleMapEntryHovered);
		mapEntry->myOnClicked.AddUniqueDynamic(this, &UUIMapListPanel::HandleMapEntryClicked);

		myMapEntryWidgets.Add(mapEntry);
		myMapListContainer->AddChild(mapEntry);
	}
}

void UUIMapListPanel::BuildModeList(const UMapDataAsset* aMapData)
{
	ClearModeList();

	if (!aMapData || !myModeEntryWidgetClass) {
		return;
	}

	for (int32 modeIndex = 0; modeIndex < aMapData->mySupportedModes.Num(); ++modeIndex) {
		const FMenuModeEntry& modeEntryData = aMapData->mySupportedModes[modeIndex];

		UUIGameModeItem* modeEntry = CreateWidget<UUIGameModeItem>(GetOwningPlayer(), myModeEntryWidgetClass);
		if (!modeEntry) {
			continue;
		}

		modeEntry->InitializeModeEntry(modeIndex, modeEntryData.myDisplayName, modeEntryData.IsPlayable());
		modeEntry->myOnClicked.AddUniqueDynamic(this, &UUIMapListPanel::HandleModeEntryClicked);

		myModeEntryWidgets.Add(modeEntry);
		myModeListContainer->AddChild(modeEntry);
	}
}

void UUIMapListPanel::ClearMapList()
{
	for (UUIMapItem* mapEntry : myMapEntryWidgets) {
		if (!mapEntry) {
			continue;
		}

		mapEntry->myOnHovered.RemoveDynamic(this, &UUIMapListPanel::HandleMapEntryHovered);
		mapEntry->myOnClicked.RemoveDynamic(this, &UUIMapListPanel::HandleMapEntryClicked);
	}

	myMapEntryWidgets.Reset();

	myMapListContainer->ClearChildren();
}

void UUIMapListPanel::ClearModeList()
{
	for (UUIGameModeItem* modeEntry : myModeEntryWidgets) {
		if (!modeEntry) {
			continue;
		}

		modeEntry->myOnClicked.RemoveDynamic(this, &UUIMapListPanel::HandleModeEntryClicked);
	}

	myModeEntryWidgets.Reset();

	myModeListContainer->ClearChildren();
}

void UUIMapListPanel::PreviewMap(int32 anEntryIndex)
{
	if (!IsValidMapIndex(anEntryIndex)) {
		return;
	}

	myPreviewedMapIndex = anEntryIndex;
	UMapDataAsset* mapData = myAvailableMaps[myPreviewedMapIndex];
	RefreshMapDetails(mapData);

	if (mySelectedMapIndex == INDEX_NONE) {
		BuildModeList(mapData);
		RefreshModeEntrySelection();
	}
}

void UUIMapListPanel::SelectMap(int32 anEntryIndex)
{
	if (!IsValidMapIndex(anEntryIndex)) {
		return;
	}

	mySelectedMapIndex = anEntryIndex;
	myPreviewedMapIndex = anEntryIndex;

	UMapDataAsset* selectedMap = myAvailableMaps[mySelectedMapIndex];
	mySelectedModeIndex = FindFirstPlayableModeIndex(selectedMap);

	RefreshMapDetails(selectedMap);
	BuildModeList(selectedMap);
	RefreshMapEntrySelection();
	RefreshModeEntrySelection();
	RefreshStartState();
}

void UUIMapListPanel::SelectMode(int32 anEntryIndex)
{
	if (!IsValidMapIndex(mySelectedMapIndex)) {
		return;
	}

	UMapDataAsset* selectedMap = myAvailableMaps[mySelectedMapIndex];
	if (!IsValidModeIndex(selectedMap, anEntryIndex)) {
		return;
	}

	if (!selectedMap->mySupportedModes[anEntryIndex].IsPlayable()) {
		return;
	}

	mySelectedModeIndex = anEntryIndex;
	RefreshModeEntrySelection();
	RefreshStartState();
}

void UUIMapListPanel::RefreshMapDetails(const UMapDataAsset* aMapData)
{
	myMapNameText->SetText(aMapData ? aMapData->myDisplayName : FText::GetEmpty());
	myMapDescriptionText->SetText(aMapData ? aMapData->myDescription : FText::GetEmpty());

	const bool hasPreviewImage = aMapData && aMapData->myPreviewImage;

	if (hasPreviewImage) {
		myPreviewImage->SetBrushFromTexture(aMapData->myPreviewImage);
	}
}

void UUIMapListPanel::RefreshMapEntrySelection()
{
	for (int32 mapIndex = 0; mapIndex < myMapEntryWidgets.Num(); ++mapIndex) {
		if (myMapEntryWidgets[mapIndex]) {
			myMapEntryWidgets[mapIndex]->SetSelected(mapIndex == mySelectedMapIndex);
		}
	}
}

void UUIMapListPanel::RefreshModeEntrySelection()
{
	for (int32 modeIndex = 0; modeIndex < myModeEntryWidgets.Num(); ++modeIndex) {
		if (myModeEntryWidgets[modeIndex]) {
			myModeEntryWidgets[modeIndex]->SetSelected(modeIndex == mySelectedModeIndex);
		}
	}
}

void UUIMapListPanel::RefreshStartState()
{
	const bool hasValidMap = IsValidMapIndex(mySelectedMapIndex);
	UMapDataAsset* selectedMap = hasValidMap ? myAvailableMaps[mySelectedMapIndex] : nullptr;
	const bool hasPlayableMode = IsValidModeIndex(selectedMap, mySelectedModeIndex)
		&& selectedMap->mySupportedModes[mySelectedModeIndex].IsPlayable();

	myStartButton->SetButtonEnabled(hasValidMap && hasPlayableMode);
}

int32 UUIMapListPanel::FindFirstPlayableModeIndex(const UMapDataAsset* aMapData) const
{
	if (!aMapData) {
		return INDEX_NONE;
	}

	for (int32 modeIndex = 0; modeIndex < aMapData->mySupportedModes.Num(); ++modeIndex) {
		if (aMapData->mySupportedModes[modeIndex].IsPlayable()) {
			return modeIndex;
		}
	}

	return INDEX_NONE;
}

bool UUIMapListPanel::IsValidMapIndex(int32 anEntryIndex) const
{
	return myAvailableMaps.IsValidIndex(anEntryIndex) && myAvailableMaps[anEntryIndex] != nullptr;
}

bool UUIMapListPanel::IsValidModeIndex(const UMapDataAsset* aMapData, int32 anEntryIndex) const
{
	return aMapData && aMapData->mySupportedModes.IsValidIndex(anEntryIndex);
}
