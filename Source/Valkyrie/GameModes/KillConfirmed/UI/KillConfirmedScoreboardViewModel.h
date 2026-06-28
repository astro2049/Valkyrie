// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/PVP/UI/PVPScoreboardViewModel.h"
#include "KillConfirmedScoreboardViewModel.generated.h"

UCLASS()
class VALKYRIE_API UKillConfirmedScoreboardViewModel : public UPVPScoreboardViewModel
{
	GENERATED_BODY()

public:
	virtual bool ShouldShowConfirms() const override { return true; }

protected:
	virtual bool BuildRowData(const APlayerState* aPlayerState, FPVPScoreboardRowData& aRowData) const override;
	virtual void BuildScoreText(FText& aTeamAScoreText, FText& aTeamBScoreText) const override;
};
