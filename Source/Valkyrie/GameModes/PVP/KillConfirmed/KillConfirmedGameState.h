// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/PVP/PVPGameState.h"
#include "KillConfirmedGameState.generated.h"

UCLASS()
class VALKYRIE_API AKillConfirmedGameState : public APVPGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	int32 AddTeamConfirm(EValkTeamId aTeamId);
	int32 GetTeamAConfirms() const { return myTeamAConfirms; }
	int32 GetTeamBConfirms() const { return myTeamBConfirms; }

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myTeamAConfirms{0};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myTeamBConfirms{0};
};
