// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "TargetPawn.generated.h"

class UHealthComponent;

UCLASS(Blueprintable)
class VALKYRIE_API ATargetPawn : public APawn
{
	GENERATED_BODY()

public:
	ATargetPawn();
	void OnDied();

private:
	void Respawn();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myRespawnDelay{2.f};
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UHealthComponent> myHealthComponent{nullptr};
	FTimerHandle myRespawnTimerHandle;
};
