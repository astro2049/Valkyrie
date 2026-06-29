// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/ValkGameState.h"
#include "PVPTypes.h"
#include "PVPGameState.generated.h"

UCLASS()
class VALKYRIE_API APVPGameState : public AValkGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AddPlayerState(APlayerState* aPlayerState) override;
	virtual void RemovePlayerState(APlayerState* aPlayerState) override;

	void SetWinningTeamId(int32 aTeamId);
	UFUNCTION()
	void BroadcastStateChanged() const;
	virtual bool HasMatchEnded() const override { return myMatchEnded; }
	int32 GetWinningTeamId() const { return myWinningTeamId; }

private:
	UPROPERTY(ReplicatedUsing=BroadcastStateChanged, VisibleAnywhere, Category="Valkyrie")
	bool myMatchEnded{false};
	UPROPERTY(ReplicatedUsing=BroadcastStateChanged, VisibleAnywhere, Category="Valkyrie")
	int32 myWinningTeamId{ValkTeamId::None};
};
