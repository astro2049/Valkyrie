// Fill out your copyright notice in the Description page of Project Settings.

#include "UIGameModeItem.h"

#include "Valkyrie/UI/UIButton.h"

void UUIGameModeItem::InitializeModeEntry(int32 anEntryIndex, const FText& aDisplayName, bool aIsPlayable)
{
	myEntryIndex = anEntryIndex;
	myIsPlayable = aIsPlayable;

	const FText label = myIsPlayable
		                    ? aDisplayName
		                    : FText::Format(NSLOCTEXT("Menu", "ComingSoonModeLabel", "{0} (Coming Soon)"), aDisplayName);

	myButton->SetLabel(label);
	myButton->SetButtonEnabled(myIsPlayable);
}

void UUIGameModeItem::SetSelected(bool aIsSelected)
{
	HandleSelectedChanged(aIsSelected);
}

void UUIGameModeItem::NativeConstruct()
{
	Super::NativeConstruct();

	BindButton();
}

void UUIGameModeItem::NativeDestruct()
{
	UnbindButton();

	Super::NativeDestruct();
}

void UUIGameModeItem::HandleButtonClicked()
{
	if (!myIsPlayable) {
		return;
	}

	myOnClicked.Broadcast(myEntryIndex);
}

void UUIGameModeItem::BindButton()
{
	myButton->myOnClicked.AddUniqueDynamic(this, &UUIGameModeItem::HandleButtonClicked);
}

void UUIGameModeItem::UnbindButton()
{
	myButton->myOnClicked.RemoveDynamic(this, &UUIGameModeItem::HandleButtonClicked);
}
