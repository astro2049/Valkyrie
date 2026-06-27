// Fill out your copyright notice in the Description page of Project Settings.

#include "UIExtractionHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "ExtractionHUDViewModel.h"

void UUIExtractionHUD::SetViewModel(UExtractionHUDViewModel* aViewModel)
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
		myViewModel->myOnViewModelChanged.AddUObject(this, &UUIExtractionHUD::HandleViewModelChanged);
	}

	UpdateFromViewModel();
}

void UUIExtractionHUD::UpdateFromViewModel() const
{
	if (!myViewModel) {
		return;
	}
	
	// weapon
	const FValkWeaponHUDData& weaponHUDData = myViewModel->GetWeaponHUDData();
	const bool showAmmo = weaponHUDData.myShowAmmo;
	if (myAmmoText) {
		myAmmoText->SetText(showAmmo ? FText::AsNumber(weaponHUDData.myAmmoInMag) : FText::GetEmpty());
	}
	if (myReserveAmmoText) {
		myReserveAmmoText->SetText(showAmmo ? FText::AsNumber(weaponHUDData.myReserveAmmo) : FText::GetEmpty());
	}
	if (myReloadingIndicator) {
		myReloadingIndicator->SetVisibility(weaponHUDData.myIsReloading ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	// health
	const FValkHealthHUDData& healthHUDData = myViewModel->GetHealthHUDData();
	const bool showHealth = healthHUDData.myShowHealth;
	if (myHealthBar) {
		myHealthBar->SetVisibility(showHealth ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		myHealthBar->SetPercent(showHealth ? myViewModel->GetHealthRatio() : 0.f);
	}
	if (myHealthText) {
		myHealthText->SetVisibility(showHealth ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (showHealth) {
			const int32 health = FMath::CeilToInt(healthHUDData.myHealth);
			const int32 maxHealth = FMath::CeilToInt(healthHUDData.myMaxHealth);
			myHealthText->SetText(FText::Format(
				NSLOCTEXT("CombatHUD", "HealthFormat", "{0} / {1}"),
				FText::AsNumber(health),
				FText::AsNumber(maxHealth)
			));
		} else {
			myHealthText->SetText(FText::GetEmpty());
		}
	}

	// objective, defense timer
	const FExtractionHUDData& extractionHUDData = myViewModel->GetExtractionHUDData();
	if (myObjectiveText) {
		myObjectiveText->SetText(extractionHUDData.myObjectiveText);
	}
	if (myDefenseTimerText) {
		const bool showDefenseTimer = extractionHUDData.myShowDefenseTimer;
		myDefenseTimerText->SetVisibility(showDefenseTimer ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		myDefenseTimerText->SetText(showDefenseTimer
			? FText::AsNumber(FMath::CeilToInt(extractionHUDData.myDefenseTimeRemaining))
			: FText::GetEmpty());
	}

	// interact prompt
	if (myInteractPrompt) {
		myInteractPrompt->SetVisibility(myViewModel->GetShowInteractPrompt() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUIExtractionHUD::HandleViewModelChanged()
{
	UpdateFromViewModel();
}
