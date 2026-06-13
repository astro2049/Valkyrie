// Fill out your copyright notice in the Description page of Project Settings.

#include "UIButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUIButton::SetLabel(const FText& aLabel)
{
	myLabel = aLabel;

	if (myText) {
		myText->SetText(myLabel);
	}
}

void UUIButton::SetButtonEnabled(bool aIsEnabled)
{
	if (myButton) {
		myButton->SetIsEnabled(aIsEnabled);
	}
}

void UUIButton::NativeConstruct()
{
	Super::NativeConstruct();

	BindButton();
	SetLabel(myLabel);
}

void UUIButton::NativeDestruct()
{
	UnbindButton();

	Super::NativeDestruct();
}

void UUIButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	SetLabel(myLabel);
}

void UUIButton::HandleButtonClicked()
{
	myOnClicked.Broadcast();
}

void UUIButton::HandleButtonHovered()
{
	myOnHovered.Broadcast();
}

void UUIButton::BindButton()
{
	if (!myButton) {
		return;
	}

	myButton->OnClicked.AddUniqueDynamic(this, &UUIButton::HandleButtonClicked);
	myButton->OnHovered.AddUniqueDynamic(this, &UUIButton::HandleButtonHovered);
}

void UUIButton::UnbindButton()
{
	if (!myButton) {
		return;
	}

	myButton->OnClicked.RemoveDynamic(this, &UUIButton::HandleButtonClicked);
	myButton->OnHovered.RemoveDynamic(this, &UUIButton::HandleButtonHovered);
}
