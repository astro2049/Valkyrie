// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetPawn.h"

#include "TimerManager.h"
#include "Valkyrie/Components/HealthComponent.h"

ATargetPawn::ATargetPawn()
{
	myHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("myHealthComponent"));
}

void ATargetPawn::OnDied()
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
