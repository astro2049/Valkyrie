// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetHealth();
}

void UHealthComponent::ApplyDamage(float aDamage)
{
	if (myIsDead || aDamage <= 0.f) {
		return;
	}

	myHealth = FMath::Clamp(myHealth - aDamage, 0.f, myMaxHealth);
	OnHealthChanged.Broadcast(myHealth, myMaxHealth);

	if (myHealth <= 0.f) {
		myIsDead = true;
		OnDeath.Broadcast();
	}
}

void UHealthComponent::ResetHealth()
{
	myIsDead = false;
	myHealth = myMaxHealth;
	OnHealthChanged.Broadcast(myHealth, myMaxHealth);
}
