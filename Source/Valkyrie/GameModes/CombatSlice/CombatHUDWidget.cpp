// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatHUDWidget.h"

#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Valkyrie/Player/PlayerCombatViewModel.h"

void UCombatHUDWidget::SetViewModel(UPlayerCombatViewModel* aViewModel)
{
	if (myViewModel == aViewModel) {
		UpdateFromViewModel();
		return;
	}

	if (myViewModel) {
		myViewModel->myOnViewModelChanged.RemoveDynamic(
			this,
			&UCombatHUDWidget::HandleViewModelChanged
		);
	}

	myViewModel = aViewModel;

	if (myViewModel) {
		myViewModel->myOnViewModelChanged.AddUniqueDynamic(
			this,
			&UCombatHUDWidget::HandleViewModelChanged
		);
	}

	UpdateFromViewModel();
}

void UCombatHUDWidget::UpdateFromViewModel()
{
	const bool showAmmo = myViewModel && myViewModel->ShowAmmo();

	if (myAmmoText) {
		myAmmoText->SetText(showAmmo ? FText::AsNumber(myViewModel->GetAmmoInMag()) : FText::GetEmpty());
	}

	if (myReserveAmmoText) {
		myReserveAmmoText->SetText(showAmmo ? FText::AsNumber(myViewModel->GetReserveAmmo()) : FText::GetEmpty());
	}

	if (myReloadingIndicator) {
		const bool isReloading = myViewModel && myViewModel->IsReloading();
		myReloadingIndicator->SetVisibility(isReloading ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UCombatHUDWidget::NativeDestruct()
{
	SetViewModel(nullptr);
	Super::NativeDestruct();
}

void UCombatHUDWidget::HandleViewModelChanged()
{
	UpdateFromViewModel();
}
