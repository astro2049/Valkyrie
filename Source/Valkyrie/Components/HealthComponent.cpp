// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "Net/UnrealNetwork.h"
#include "Valkyrie/Player/ValkPlayerCharacter.h"

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
			}

			OnHealthChanged.Broadcast(myHealth, myMaxHealth);
			if (myIsDead) {
				if (AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetOwner())) {
					playerCharacter->OnDied(aDamageInstigator);
				}
				OnDeath.Broadcast();
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
			OnHealthChanged.Broadcast(myHealth, myMaxHealth);
		}
	}
}

void UHealthComponent::OnRep_Health() const
{
	OnHealthChanged.Broadcast(myHealth, myMaxHealth);
}

void UHealthComponent::OnRep_IsDead() const
{
	if (myIsDead) {
		OnDeath.Broadcast();
	}
}
