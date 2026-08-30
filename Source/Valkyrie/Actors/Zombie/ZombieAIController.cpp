// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"

#include "Engine/World.h"
#include "Valkyrie/Components/HealthComponent.h"

void AZombieAIController::Attack(AActor* const aTarget)
{
	if (!aTarget) {
		return;
	}
	if (!HasAuthority()) {
		return;
	}

	if (const float currentTime = GetWorld()->GetTimeSeconds(); currentTime >= myLastAttackTime + myAttackCooldown) {
		myLastAttackTime = currentTime;
		if (UHealthComponent* const healthComponent = aTarget->FindComponentByClass<UHealthComponent>()) {
			healthComponent->ApplyDamage(myDamagePerAttack, this);
		}
	}
}
