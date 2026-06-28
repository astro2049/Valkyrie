// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/PVP/PVPGameState.h"
#include "TDMGameState.generated.h"

UCLASS()
class VALKYRIE_API ATDMGameState : public APVPGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	int32 AddTeamKill(int32 aTeamId);
	int32 GetTeamAKills() const { return myTeamAKills; }
	int32 GetTeamBKills() const { return myTeamBKills; }

private:
	UFUNCTION()
	void OnRep_TeamKills();

	UPROPERTY(ReplicatedUsing=OnRep_TeamKills, VisibleAnywhere, Category="Valkyrie")
	int32 myTeamAKills{0};
	UPROPERTY(ReplicatedUsing=OnRep_TeamKills, VisibleAnywhere, Category="Valkyrie")
	int32 myTeamBKills{0};
};
