// Fill out your copyright notice in the Description page of Project Settings.

#include "UIPVPHUD.h"

#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "PVPHUDViewModel.h"
#include "Valkyrie/UI/UIMessageSubsystem.h"

void UUIPVPHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (!myViewModel && myViewModelClass) {
		myViewModel = NewObject<UPVPHUDViewModel>(this, myViewModelClass);
		if (myViewModel) {
			myViewModel->Initialize(GetOwningPlayer());
		}
	}
	RefreshModeData();
}

void UUIPVPHUD::NativeTick(const FGeometry& aGeometry, const float aDeltaSeconds)
{
	Super::NativeTick(aGeometry, aDeltaSeconds);

	const UUIMessageSubsystem* const messageSubsystem = VALK_UISUBSYS();
	if (messageSubsystem
		&& messageSubsystem->CheckUIMessage(EUIMessageType::GameStateUpdated)) {
		RefreshModeData();
	}
}

void UUIPVPHUD::RefreshModeData()
{
	if (myViewModel) {
		myViewModel->RefreshData();
	}
	UpdateFromViewModel();
}

void UUIPVPHUD::UpdateFromViewModel() const
{
	if (!myViewModel) {
		return;
	}

	const FPVPHUDData& pvpHUDData = myViewModel->GetPVPHUDData();
	if (myTeamAScoreText) {
		myTeamAScoreText->SetText(pvpHUDData.myTeamAScoreText);
	}
	if (myTeamBScoreText) {
		myTeamBScoreText->SetText(pvpHUDData.myTeamBScoreText);
	}
	if (myWinnerText) {
		myWinnerText->SetText(pvpHUDData.myWinnerText);
		myWinnerText->SetVisibility(pvpHUDData.myShowWinner ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
