// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"
#include "KillConfirmedPlayerState.generated.h"

UCLASS()
class VALKYRIE_API AKillConfirmedPlayerState : public AValkPlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddKill();
	void AddDeath();
	void AddConfirm();
	int32 GetKills() const { return myKills; }
	int32 GetDeaths() const { return myDeaths; }
	int32 GetConfirms() const { return myConfirms; }

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myKills{0};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myDeaths{0};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myConfirms{0};
};
