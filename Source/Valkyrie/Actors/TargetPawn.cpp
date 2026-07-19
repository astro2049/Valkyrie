// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetPawn.h"

#include "TimerManager.h"
#include "Valkyrie/Components/HealthComponent.h"

ATargetPawn::ATargetPawn()
{
	myHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("myHealthComponent"));
}

void ATargetPawn::BeginPlay()
{
	Super::BeginPlay();

	if (myHealthComponent) {
		myHealthComponent->OnDied().BindUObject(this, &ATargetPawn::OnHealthDied);
	}
}

void ATargetPawn::OnHealthDied(AController* const)
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	GetWorldTimerManager().SetTimer(
		myRespawnTimerHandle,
		this,
		&ATargetPawn::Respawn,
		myRespawnDelay,
		false
	);
}

void ATargetPawn::Respawn()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	myHealthComponent->ResetHealth();
}
