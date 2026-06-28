// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/UI/ValkViewModel.h"
#include "PVPScoreboardViewModel.generated.h"

class APVPGameState;
class APlayerState;
class AValkPlayerState;

struct FPVPScoreboardRowData
{
	FText myPlayerNameText;
	FText myConfirmsText;
	FText myKillsText;
	FText myDeathsText;
	int32 myKills{0};
	int32 myDeaths{0};
};

struct FPVPScoreboardData
{
	FText myTeamAScoreText;
	FText myTeamBScoreText;
	TArray<FPVPScoreboardRowData> myTeamARows;
	TArray<FPVPScoreboardRowData> myTeamBRows;
};

UCLASS(Abstract)
class VALKYRIE_API UPVPScoreboardViewModel : public UValkViewModel
{
	GENERATED_BODY()

public:
	void BindToGameState(APVPGameState* aGameState);
	const FPVPScoreboardData& GetScoreboardData() const { return myScoreboardData; }
	virtual bool ShouldShowConfirms() const { return false; }

protected:
	virtual bool BuildRowData(const APlayerState* aPlayerState, FPVPScoreboardRowData& aRowData) const PURE_VIRTUAL(UPVPScoreboardViewModel::BuildRowData, return false;);
	virtual void BuildScoreText(FText& aTeamAScoreText, FText& aTeamBScoreText) const PURE_VIRTUAL(UPVPScoreboardViewModel::BuildScoreText, );
	APVPGameState* GetPVPGameState() const { return myPVPGameState.Get(); }

private:
	void HandleGameStateChanged();
	void HandlePlayerStateChanged();
	void RebindPlayerStates();
	void RebuildScoreboardData();

	TWeakObjectPtr<APVPGameState> myPVPGameState;
	TArray<TWeakObjectPtr<AValkPlayerState>> myBoundPlayerStates;
	FPVPScoreboardData myScoreboardData;
};
