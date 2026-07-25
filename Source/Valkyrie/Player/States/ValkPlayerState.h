// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Valkyrie/Common/ValkTypes.h"
#include "ValkPlayerState.generated.h"

UCLASS()
class VALKYRIE_API AValkPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void SetTeamId(const EValkTeamId aTeamId) { myTeamId = aTeamId; }
	EValkTeamId GetTeamId() const { return myTeamId; }
	void SetIndexInTeam(const int aIndex) { myIndexInTeam = aIndex; }
	int GetIndexInTeam() const { return myIndexInTeam; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	EValkTeamId myTeamId{EValkTeamId::None};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int myIndexInTeam{-1};
};
