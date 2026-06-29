// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/PVP/Player/PVPPlayerState.h"
#include "TDMPlayerState.generated.h"

UCLASS()
class VALKYRIE_API ATDMPlayerState : public APVPPlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddKill();
	void AddDeath();
	int32 GetKills() const { return myKills; }
	int32 GetDeaths() const { return myDeaths; }

private:
	UPROPERTY(ReplicatedUsing=BroadcastStateChanged, VisibleAnywhere, Category="Valkyrie")
	int32 myKills{0};
	UPROPERTY(ReplicatedUsing=BroadcastStateChanged, VisibleAnywhere, Category="Valkyrie")
	int32 myDeaths{0};
};
