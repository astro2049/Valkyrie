// Fill out your copyright notice in the Description page of Project Settings.

#include "UIMapItem.h"

#include "Valkyrie/UI/UIButton.h"

void UUIMapItem::InitializeMapEntry(int32 anEntryIndex, const FText& aDisplayName)
{
	myEntryIndex = anEntryIndex;

	myButton->SetLabel(aDisplayName);
	myButton->SetButtonEnabled(true);
}

void UUIMapItem::SetSelected(bool aIsSelected)
{
	HandleSelectedChanged(aIsSelected);
}

void UUIMapItem::NativeConstruct()
{
	Super::NativeConstruct();

	BindButton();
}

void UUIMapItem::NativeDestruct()
{
	UnbindButton();

	Super::NativeDestruct();
}

void UUIMapItem::HandleButtonHovered()
{
	myOnHovered.Broadcast(myEntryIndex);
}

void UUIMapItem::HandleButtonClicked()
{
	myOnClicked.Broadcast(myEntryIndex);
}

void UUIMapItem::BindButton()
{
	myButton->myOnHovered.AddUniqueDynamic(this, &UUIMapItem::HandleButtonHovered);
	myButton->myOnClicked.AddUniqueDynamic(this, &UUIMapItem::HandleButtonClicked);
}

void UUIMapItem::UnbindButton()
{
	myButton->myOnHovered.RemoveDynamic(this, &UUIMapItem::HandleButtonHovered);
	myButton->myOnClicked.RemoveDynamic(this, &UUIMapItem::HandleButtonClicked);
}
