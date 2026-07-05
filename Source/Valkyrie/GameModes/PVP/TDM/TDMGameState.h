// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/ValkGameState.h"
#include "TDMGameState.generated.h"

UCLASS()
class VALKYRIE_API ATDMGameState : public AValkGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	int32 AddTeamKill(EValkTeamId aTeamId);
	int32 GetTeamAKills() const { return myTeamAKills; }
	int32 GetTeamBKills() const { return myTeamBKills; }

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myTeamAKills{0};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myTeamBKills{0};
};
