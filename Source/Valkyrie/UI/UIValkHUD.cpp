// Fill out your copyright notice in the Description page of Project Settings.

#include "UIValkHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Engine/World.h"
#include "UIMessageSubsystem.h"
#include "ValkHUDViewModel.h"

void UUIValkHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (!myViewModel && myViewModelClass) {
		myViewModel = NewObject<UValkHUDViewModel>(this, myViewModelClass);
		if (myViewModel) {
			myViewModel->Initialize(GetOwningPlayer());
		}
	}
	RefreshHUDData();
}

void UUIValkHUD::RefreshHUDData()
{
	if (myViewModel) {
		myViewModel->RefreshData();
	}
	UpdateFromViewModel();
}

void UUIValkHUD::NativeTick(const FGeometry& aGeometry, const float aDeltaSeconds)
{
	Super::NativeTick(aGeometry, aDeltaSeconds);

	const UUIMessageSubsystem* const messageSubsystem = VALK_UISUBSYS();
	if (messageSubsystem && (
		messageSubsystem->CheckUIMessage(EUIMessageType::WeaponComponentUpdated)
		|| messageSubsystem->CheckUIMessage(EUIMessageType::HealthComponentUpdated)
		|| messageSubsystem->CheckUIMessage(EUIMessageType::InteractionComponentUpdated)
		|| messageSubsystem->CheckUIMessage(EUIMessageType::GameStateUpdated)
	)) {
		RefreshHUDData();
	}
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
