// Fill out your copyright notice in the Description page of Project Settings.

#include "UIPVPScoreboard.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "PVPScoreboardViewModel.h"
#include "UIPVPScoreboardRow.h"

void UUIPVPScoreboard::SetViewModel(UPVPScoreboardViewModel* const aViewModel)
{
	if (myViewModel == aViewModel) {
		RebuildRows();
		return;
	}

	if (myViewModel) {
		myViewModel->myOnViewModelChanged.RemoveAll(this);
	}
	myViewModel = aViewModel;
	if (myViewModel) {
		myViewModel->myOnViewModelChanged.AddUObject(this, &UUIPVPScoreboard::HandleViewModelChanged);
	}
	RebuildRows();
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

void UUIPVPScoreboard::HandleViewModelChanged()
{
	if (GetVisibility() == ESlateVisibility::Visible) {
		RebuildRows();
	}
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
