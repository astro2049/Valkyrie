// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetActor.h"

#include "TimerManager.h"
#include "Valkyrie/Components/HealthComponent.h"

ATargetActor::ATargetActor()
{
	myHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("myHealthComponent"));
}

void ATargetActor::BeginPlay()
{
	Super::BeginPlay();

	if (myHealthComponent) {
		myHealthComponent->GetOnDied().BindUObject(this, &ATargetActor::OnHealthDied);
	}
}

void ATargetActor::OnHealthDied(AController* const)
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	GetWorldTimerManager().SetTimer(
		myRespawnTimerHandle,
		this,
		&ATargetActor::Respawn,
		myRespawnDelay,
		false
	);
}

void ATargetActor::Respawn()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	myHealthComponent->Reset();
}
