// Fill out your copyright notice in the Description page of Project Settings.

#include "UIMainMenu.h"

#include "MainMenuPanel/UIMainMenuPanel.h"
#include "MapListPanel//UIMapListPanel.h"

void UUIMainMenu::ShowMainMenu()
{
	myMainMenuPanel->SetVisibility(ESlateVisibility::Visible);
	myMapListPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UUIMainMenu::ShowMapSelect()
{
	myMainMenuPanel->SetVisibility(ESlateVisibility::Collapsed);
	myMapListPanel->SetVisibility(ESlateVisibility::Visible);
	myMapListPanel->ResetMapSelection();
}

void UUIMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	BindChildWidgets();
	ShowMainMenu();
}

void UUIMainMenu::NativeDestruct()
{
	UnbindChildWidgets();

	Super::NativeDestruct();
}

void UUIMainMenu::HandleSelectMapRequested()
{
	ShowMapSelect();
}

void UUIMainMenu::HandleQuitRequested()
{
	myOnQuitRequested.Broadcast();
}

void UUIMainMenu::HandleBackRequested()
{
	ShowMainMenu();
}

void UUIMainMenu::HandleStartRequested(UMapDataAsset* aMapData, FMenuModeEntry aModeEntry)
{
	myOnStartRequested.Broadcast(aMapData, aModeEntry);
}

void UUIMainMenu::BindChildWidgets()
{
	myMainMenuPanel->myOnSelectMapRequested.AddUniqueDynamic(this, &UUIMainMenu::HandleSelectMapRequested);
	myMainMenuPanel->myOnQuitRequested.AddUniqueDynamic(this, &UUIMainMenu::HandleQuitRequested);
	myMapListPanel->myOnBackRequested.AddUniqueDynamic(this, &UUIMainMenu::HandleBackRequested);
	myMapListPanel->myOnStartRequested.AddUniqueDynamic(this, &UUIMainMenu::HandleStartRequested);
}

void UUIMainMenu::UnbindChildWidgets()
{
	myMainMenuPanel->myOnSelectMapRequested.RemoveDynamic(this, &UUIMainMenu::HandleSelectMapRequested);
	myMainMenuPanel->myOnQuitRequested.RemoveDynamic(this, &UUIMainMenu::HandleQuitRequested);
	myMapListPanel->myOnBackRequested.RemoveDynamic(this, &UUIMainMenu::HandleBackRequested);
	myMapListPanel->myOnStartRequested.RemoveDynamic(this, &UUIMainMenu::HandleStartRequested);
}
