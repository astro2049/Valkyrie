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
	void SetTeamId(const int32 aTeamId) { myTeamId = aTeamId; }
	int32 GetTeamId() const { return myTeamId; }
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myTeamId{ValkTeamId::None};
};
