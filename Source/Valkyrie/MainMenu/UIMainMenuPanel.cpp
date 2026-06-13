// Fill out your copyright notice in the Description page of Project Settings.

#include "UIMainMenuPanel.h"

#include "Valkyrie/UI/UIButton.h"

void UUIMainMenuPanel::NativeConstruct()
{
	Super::NativeConstruct();

	BindButtons();
}

void UUIMainMenuPanel::NativeDestruct()
{
	UnbindButtons();

	Super::NativeDestruct();
}

void UUIMainMenuPanel::HandleSelectMapClicked()
{
	myOnSelectMapRequested.Broadcast();
}

void UUIMainMenuPanel::HandleQuitClicked()
{
	myOnQuitRequested.Broadcast();
}

void UUIMainMenuPanel::BindButtons()
{
	mySelectMapButton->myOnClicked.AddUniqueDynamic(this, &UUIMainMenuPanel::HandleSelectMapClicked);
	myQuitButton->myOnClicked.AddUniqueDynamic(this, &UUIMainMenuPanel::HandleQuitClicked);
}

void UUIMainMenuPanel::UnbindButtons()
{
	mySelectMapButton->myOnClicked.RemoveDynamic(this, &UUIMainMenuPanel::HandleSelectMapClicked);
	myQuitButton->myOnClicked.RemoveDynamic(this, &UUIMainMenuPanel::HandleQuitClicked);
}
