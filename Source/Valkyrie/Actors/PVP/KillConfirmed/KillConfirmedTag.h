// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Valkyrie/GameModes/PVP/PVPTypes.h"
#include "KillConfirmedTag.generated.h"

UCLASS()
class VALKYRIE_API AKillConfirmedTag : public AActor
{
	GENERATED_BODY()

public:
	AKillConfirmedTag();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetDroppedTeamId(int32 aTeamId);
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void HandlePlayerEntered(AActor* anOtherActor);

private:
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	int32 myDroppedTeamId{ValkTeamId::None};
};
