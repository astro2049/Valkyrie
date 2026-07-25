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

	if (const AActor* const owner = GetOwner(); owner && owner->HasAuthority()) {
		Reset();
	}
}

void UHealthComponent::Reset()
{
	myHealth = myMaxHealth;
	myIsDead = false;
}

void UHealthComponent::ApplyDamage(const float aDamage, AController* const aDamageInstigator)
{
	if (aDamage <= 0.f || myIsDead) {
		return;
	}

	myHealth = FMath::Clamp(myHealth - aDamage, 0.f, myMaxHealth);
	myDamagedDelegate.ExecuteIfBound(aDamage, aDamageInstigator);
	if (myHealth <= 0.f) {
		myIsDead = true;
		myDiedDelegate.ExecuteIfBound(aDamageInstigator);
	}
}
