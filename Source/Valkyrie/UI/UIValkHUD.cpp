// Fill out your copyright notice in the Description page of Project Settings.

#include "UIValkHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "ValkHUDViewModel.h"

void UUIValkHUD::SetViewModel(UValkHUDViewModel* const aViewModel)
{
	if (myViewModel == aViewModel) {
		UpdateFromViewModel();
		return;
	}

	if (myViewModel) {
		myViewModel->myOnViewModelChanged.RemoveAll(this);
	}
	myViewModel = aViewModel;
	if (myViewModel) {
		myViewModel->myOnViewModelChanged.AddUObject(this, &UUIValkHUD::HandleViewModelChanged);
	}
	UpdateFromViewModel();
}

void UUIValkHUD::UpdateFromViewModel() const
{
	if (!myViewModel) {
		return;
	}

	const FValkWeaponHUDData& weaponHUDData = myViewModel->GetWeaponHUDData();
	if (myAmmoText) {
		myAmmoText->SetText(weaponHUDData.myShowAmmo ? FText::AsNumber(weaponHUDData.myAmmoInMag) : FText::GetEmpty());
	}
	if (myReserveAmmoText) {
		myReserveAmmoText->SetText(weaponHUDData.myShowAmmo ? FText::AsNumber(weaponHUDData.myReserveAmmo) : FText::GetEmpty());
	}
	if (myReloadingIndicator) {
		myReloadingIndicator->SetVisibility(weaponHUDData.myIsReloading ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	const FValkHealthHUDData& healthHUDData = myViewModel->GetHealthHUDData();
	if (myHealthBar) {
		myHealthBar->SetVisibility(healthHUDData.myShowHealth ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		myHealthBar->SetPercent(healthHUDData.myShowHealth ? myViewModel->GetHealthRatio() : 0.f);
	}
	if (myHealthText) {
		myHealthText->SetVisibility(healthHUDData.myShowHealth ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		myHealthText->SetText(healthHUDData.myShowHealth
			? FText::Format(
				NSLOCTEXT("ValkHUD", "HealthFormat", "{0} / {1}"),
				FText::AsNumber(FMath::CeilToInt(healthHUDData.myHealth)),
				FText::AsNumber(FMath::CeilToInt(healthHUDData.myMaxHealth))
			)
			: FText::GetEmpty());
	}
	if (myInteractPrompt) {
		myInteractPrompt->SetVisibility(myViewModel->GetShowInteractPrompt() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	UpdateModeFromViewModel();
}

void UUIValkHUD::HandleViewModelChanged()
{
	UpdateFromViewModel();
}
