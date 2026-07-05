// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/ValkGameState.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "PVPGameState.generated.h"

UCLASS()
class VALKYRIE_API APVPGameState : public AValkGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetWinningTeamId(EValkTeamId aTeamId);
	virtual bool HasMatchEnded() const override { return myMatchEnded; }
	EValkTeamId GetWinningTeamId() const { return myWinningTeamId; }

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	bool myMatchEnded{false};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	EValkTeamId myWinningTeamId{EValkTeamId::None};
};
