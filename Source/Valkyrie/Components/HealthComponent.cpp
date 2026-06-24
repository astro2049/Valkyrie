// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, myHealth);
	DOREPLIFETIME(UHealthComponent, myIsDead);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetHealth();
}

void UHealthComponent::ApplyDamage(float aDamage)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) {
		return;
	}

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

void UHealthComponent::OnRep_Health()
{
	OnHealthChanged.Broadcast(myHealth, myMaxHealth);
}

void UHealthComponent::OnRep_IsDead()
{
	if (myIsDead) {
		OnDeath.Broadcast();
	}
}
