// Fill out your copyright notice in the Description page of Project Settings.

#include "UIPVPScoreboard.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Engine/World.h"
#include "PVPScoreboardViewModel.h"
#include "UIPVPScoreboardRow.h"
#include "Valkyrie/UI/UIMessageSubsystem.h"

void UUIPVPScoreboard::NativeConstruct()
{
	Super::NativeConstruct();

	if (!myViewModel && myViewModelClass) {
		myViewModel = NewObject<UPVPScoreboardViewModel>(this, myViewModelClass);
		if (myViewModel) {
			myViewModel->Initialize(GetOwningPlayer());
		}
	}
	RefreshScoreboardData();
}

void UUIPVPScoreboard::RefreshScoreboardData()
{
	if (myViewModel) {
		myViewModel->RefreshData();
	}
	RebuildRows();
}

void UUIPVPScoreboard::NativeTick(const FGeometry& aGeometry, const float aDeltaSeconds)
{
	Super::NativeTick(aGeometry, aDeltaSeconds);

	const UUIMessageSubsystem* const messageSubsystem = VALK_UISUBSYS();
	if (GetVisibility() == ESlateVisibility::Visible && messageSubsystem && (
		messageSubsystem->CheckUIMessage(EUIMessageType::GameStateUpdated)
		|| messageSubsystem->CheckUIMessage(EUIMessageType::PlayerStateUpdated)
	)) {
		RefreshScoreboardData();
	}
}

void UUIPVPScoreboard::RebuildRows()
{
	if (!myViewModel) {
		return;
	}

	const FPVPScoreboardData& scoreboardData = myViewModel->GetScoreboardData();
	if (myTeamAScoreText) {
		myTeamAScoreText->SetText(scoreboardData.myTeamAScoreText);
	}
	if (myTeamBScoreText) {
		myTeamBScoreText->SetText(scoreboardData.myTeamBScoreText);
	}
	AddRows(myTeamARows, scoreboardData.myTeamARows);
	AddRows(myTeamBRows, scoreboardData.myTeamBRows);
}

void UUIPVPScoreboard::AddRows(
	UVerticalBox* const aContainer,
	const TArray<FPVPScoreboardRowData>& aRows
) const
{
	if (!aContainer || !myRowWidgetClass) {
		return;
	}

	aContainer->ClearChildren();
	for (const FPVPScoreboardRowData& rowData : aRows) {
		if (UUIPVPScoreboardRow* const rowWidget = CreateWidget<UUIPVPScoreboardRow>(GetOwningPlayer(), myRowWidgetClass)) {
			rowWidget->SetData(rowData, myViewModel->ShouldShowConfirms());
			aContainer->AddChild(rowWidget);
		}
	}
}
