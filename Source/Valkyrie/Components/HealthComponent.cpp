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

void UHealthComponent::ApplyDamage(const float aDamage)
{
	if (aDamage <= 0.f) {
		return;
	}
	
	if (GetOwner() && GetOwner()->HasAuthority()) {
		if (!myIsDead) {
			myHealth = FMath::Clamp(myHealth - aDamage, 0.f, myMaxHealth);
			if (myHealth <= 0.f) {
				myIsDead = true;
			}

			myOnHealthStateChanged.Broadcast();
			OnHealthChanged.Broadcast(myHealth, myMaxHealth);
			if (myIsDead) {
				OnDeath.Broadcast();
			}
		}
	}
}

void UHealthComponent::ResetHealth()
{
	const AActor* const owner = GetOwner();
	if (!owner || !owner->HasAuthority()) {
		return;
	}

	myHealth = myMaxHealth;
	myIsDead = false;
	myOnHealthStateChanged.Broadcast();
	OnHealthChanged.Broadcast(myHealth, myMaxHealth);
}

void UHealthComponent::OnRep_Health() const
{
	myOnHealthStateChanged.Broadcast();
	OnHealthChanged.Broadcast(myHealth, myMaxHealth);
}

void UHealthComponent::OnRep_IsDead() const
{
	myOnHealthStateChanged.Broadcast();
	if (myIsDead) {
		OnDeath.Broadcast();
	}
}
