// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "TDMGameState.generated.h"

UCLASS()
class VALKYRIE_API ATDMGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual bool HasMatchEnded() const override { return myMatchHasEnded; }
	int32 AddTeamKill(EValkTeamId aTeamId);

	int32 GetTeamAKills() const { return myTeamAKills; }
	int32 GetTeamBKills() const { return myTeamBKills; }
	EValkTeamId GetWinningTeamId() const { return myWinningTeamId; }
	void SetMatchEnded(EValkTeamId aTeamId);

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myTeamAKills{0};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myTeamBKills{0};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	EValkTeamId myWinningTeamId{EValkTeamId::None};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	bool myMatchHasEnded{false};
};
