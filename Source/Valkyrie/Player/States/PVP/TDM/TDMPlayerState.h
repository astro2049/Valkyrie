// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"
#include "TDMPlayerState.generated.h"

UCLASS()
class VALKYRIE_API ATDMPlayerState : public AValkPlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddKill();
	void AddDeath();
	int32 GetKills() const { return myKills; }
	int32 GetDeaths() const { return myDeaths; }

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myKills{0};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myDeaths{0};
};
