// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"
#include "Valkyrie/GameModes/PVP/PVPTypes.h"
#include "PVPPlayerState.generated.h"

UCLASS()
class VALKYRIE_API APVPPlayerState : public AValkPlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetTeamId(int32 aTeamId);
	int32 GetTeamId() const { return myTeamId; }

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myTeamId{ValkTeamId::None};
};
