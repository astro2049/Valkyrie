// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/PVP/UI/PVPScoreboardViewModel.h"
#include "TDMScoreboardViewModel.generated.h"

UCLASS()
class VALKYRIE_API UTDMScoreboardViewModel : public UPVPScoreboardViewModel
{
	GENERATED_BODY()

protected:
	virtual bool BuildRowData(const APlayerState* aPlayerState, FPVPScoreboardRowData& aRowData) const override;
	virtual void BuildScoreText(FText& aTeamAScoreText, FText& aTeamBScoreText) const override;
};
