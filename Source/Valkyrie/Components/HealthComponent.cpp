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

void UHealthComponent::ApplyDamage(const float aDamage, AController* const aDamageInstigator)
{
	if (const AActor* const owner = GetOwner()) {
		if (owner->HasAuthority() && aDamage > 0.f && !myIsDead) {
			myHealth = FMath::Clamp(myHealth - aDamage, 0.f, myMaxHealth);
			if (myHealth <= 0.f) {
				myIsDead = true;
				myOnDied.ExecuteIfBound(aDamageInstigator);
			}
		}
	}
}

void UHealthComponent::ResetHealth()
{
	if (const AActor* const owner = GetOwner()) {
		if (owner->HasAuthority()) {
			myHealth = myMaxHealth;
			myIsDead = false;
		}
	}
}
