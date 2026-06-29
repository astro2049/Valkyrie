// Fill out your copyright notice in the Description page of Project Settings.

#include "UIExtractionHUD.h"

#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "ExtractionHUDViewModel.h"
#include "Valkyrie/UI/UIMessageSubsystem.h"

void UUIExtractionHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (!myViewModel && myViewModelClass) {
		myViewModel = NewObject<UExtractionHUDViewModel>(this, myViewModelClass);
		if (myViewModel) {
			myViewModel->Initialize(GetOwningPlayer());
		}
	}
	RefreshModeData();
}

void UUIExtractionHUD::NativeTick(const FGeometry& aGeometry, const float aDeltaSeconds)
{
	Super::NativeTick(aGeometry, aDeltaSeconds);

	const UUIMessageSubsystem* const messageSubsystem = VALK_UISUBSYS();
	if (messageSubsystem
		&& messageSubsystem->CheckUIMessage(EUIMessageType::GameStateUpdated)) {
		RefreshModeData();
	}
}

void UUIExtractionHUD::RefreshModeData()
{
	if (myViewModel) {
		myViewModel->RefreshData();
	}
	UpdateFromViewModel();
}

void UUIExtractionHUD::UpdateFromViewModel() const
{
	if (!myViewModel) {
		return;
	}

	const FExtractionHUDData& extractionHUDData = myViewModel->GetExtractionHUDData();
	if (myObjectiveText) {
		FText objectiveText;
		switch (extractionHUDData.myCombatSliceState) {
		case ECombatSliceState::ToStartGenerator:
			objectiveText = NSLOCTEXT("ExtractionHUD", "StartGenerator", "Objective: Start the generator");
			break;
		case ECombatSliceState::ToCompleteDefense:
			objectiveText = NSLOCTEXT("ExtractionHUD", "CompleteDefense", "Objective: Defend the generator");
			break;
		case ECombatSliceState::ToExtract:
			objectiveText = NSLOCTEXT("ExtractionHUD", "Extract", "Objective: Reach extraction");
			break;
		case ECombatSliceState::Completed:
			objectiveText = NSLOCTEXT("ExtractionHUD", "Completed", "Extraction Complete");
			break;
		case ECombatSliceState::Failed:
			objectiveText = NSLOCTEXT("ExtractionHUD", "Failed", "Mission Failed");
			break;
		}
		myObjectiveText->SetText(objectiveText);
	}
	if (myDefenseTimerText) {
		const bool showDefenseTimer = extractionHUDData.myCombatSliceState == ECombatSliceState::ToCompleteDefense;
		myDefenseTimerText->SetVisibility(showDefenseTimer ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		myDefenseTimerText->SetText(showDefenseTimer
			? FText::AsNumber(FMath::CeilToInt(extractionHUDData.myDefenseTimeRemaining))
			: FText::GetEmpty());
	}
}
