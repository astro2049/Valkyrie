// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/UI/ValkViewModel.h"
#include "PVPHUDViewModel.generated.h"

class APVPGameState;

struct FPVPHUDData
{
	FText myTeamAScoreText;
	FText myTeamBScoreText;
	FText myWinnerText;
	bool myShowWinner{false};
};

UCLASS(Abstract)
class VALKYRIE_API UPVPHUDViewModel : public UValkViewModel
{
	GENERATED_BODY()

public:
	void RefreshData();
	const FPVPHUDData& GetPVPHUDData() const { return myPVPHUDData; }

protected:
	virtual void RefreshModeHUDData() PURE_VIRTUAL(UPVPHUDViewModel::RefreshModeHUDData, );
	APVPGameState* GetPVPGameState() const;
	FPVPHUDData& GetMutablePVPHUDData() { return myPVPHUDData; }

private:
	FPVPHUDData myPVPHUDData;
};
