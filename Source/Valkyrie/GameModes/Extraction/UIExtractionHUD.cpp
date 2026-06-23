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
	const bool showAmmo = myViewModel->ShowAmmo();
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

	// health
	const bool showHealth = myViewModel && myViewModel->ShowHealth();
	if (myHealthBar) {
		myHealthBar->SetVisibility(showHealth ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		myHealthBar->SetPercent(showHealth ? myViewModel->GetHealthRatio() : 0.f);
	}
	if (myHealthText) {
		myHealthText->SetVisibility(showHealth ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (showHealth) {
			const int32 health = FMath::CeilToInt(myViewModel->GetHealth());
			const int32 maxHealth = FMath::CeilToInt(myViewModel->GetMaxHealth());
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
	if (myObjectiveText) {
		const FText objectiveText = myViewModel ? myViewModel->GetObjectiveText() : FText::GetEmpty();
		myObjectiveText->SetText(objectiveText);
	}
	if (myDefenseTimerText) {
		const bool showDefenseTimer = myViewModel && myViewModel->ShouldShowDefenseTimer();
		myDefenseTimerText->SetVisibility(showDefenseTimer ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		myDefenseTimerText->SetText(showDefenseTimer
			? FText::AsNumber(FMath::CeilToInt(myViewModel->GetDefenseTimeRemaining()))
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
