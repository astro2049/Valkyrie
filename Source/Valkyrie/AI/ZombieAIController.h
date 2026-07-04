// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

class AActor;

UCLASS(Blueprintable)
class VALKYRIE_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void Attack(AActor* aTarget);

private:
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myDamagePerAttack{10.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myAttackCooldown{1.f};

	float myLastAttackTime{0.f};
};
