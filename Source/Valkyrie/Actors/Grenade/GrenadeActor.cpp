// Fill out your copyright notice in the Description page of Project Settings.

#include "GrenadeActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Valkyrie/Components/HealthComponent.h"

AGrenadeActor::AGrenadeActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	myCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("myCollisionComponent"));
	SetRootComponent(myCollisionComponent);
	myCollisionComponent->InitSphereRadius(12.f);
	myCollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	myMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("myMeshComponent"));
	myMeshComponent->SetupAttachment(myCollisionComponent);
	myMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	myProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("myProjectileMovementComponent"));
	myProjectileMovementComponent->SetUpdatedComponent(myCollisionComponent);
	myProjectileMovementComponent->InitialSpeed = 0.f;
	myProjectileMovementComponent->MaxSpeed = 3000.f;
	myProjectileMovementComponent->ProjectileGravityScale = 1.f;
	myProjectileMovementComponent->bRotationFollowsVelocity = true;
	myProjectileMovementComponent->bShouldBounce = true;
	myProjectileMovementComponent->Bounciness = 0.35f;
	myProjectileMovementComponent->Friction = 0.2f;
}

void AGrenadeActor::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* const instigatorPawn = GetInstigator()) {
		myCollisionComponent->IgnoreActorWhenMoving(instigatorPawn, true);
	}

	if (HasAuthority()) {
		if (myFuseDuration > 0.f) {
			GetWorldTimerManager().SetTimer(
				myFuseTimerHandle,
				this,
				&AGrenadeActor::Explode,
				myFuseDuration,
				false
			);
		} else {
			Explode();
		}
	}
}

void AGrenadeActor::Launch(const FVector aDirection, const float aSpeed)
{
	const FVector launchDirection = aDirection.GetSafeNormal();
	if (!launchDirection.IsNearlyZero() && aSpeed > 0.f) {
		SetActorRotation(launchDirection.Rotation());
		myProjectileMovementComponent->Velocity = launchDirection * aSpeed;
	}
}

void AGrenadeActor::Explode()
{
	if (UWorld* const world = GetWorld()) {
		TArray<FOverlapResult> overlapResults;
		const FCollisionObjectQueryParams objectQueryParams(FCollisionObjectQueryParams::AllObjects);
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);

		world->OverlapMultiByObjectType(
			overlapResults,
			GetActorLocation(),
			FQuat::Identity,
			objectQueryParams,
			FCollisionShape::MakeSphere(myExplosionRadius),
			queryParams
		);

		TSet<AActor*> damagedActors;
		for (const FOverlapResult& overlapResult : overlapResults) {
			AActor* const actor = overlapResult.GetActor();
			if (actor && !damagedActors.Contains(actor)) {
				damagedActors.Add(actor);
				if (UHealthComponent* const healthComponent = actor->FindComponentByClass<UHealthComponent>()) {
					healthComponent->ApplyDamage(myDamage, GetInstigatorController());
				}
			}
		}
	}

	Multicast_PlayExplosionPresentation();
	SetLifeSpan(0.25f);
}

void AGrenadeActor::Multicast_PlayExplosionPresentation_Implementation()
{
	myProjectileMovementComponent->StopMovementImmediately();
	myProjectileMovementComponent->Deactivate();
	myCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	myMeshComponent->SetVisibility(false, true);
	if (myDrawExplosionDebug) {
		DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			myExplosionRadius,
			32,
			FColor::Red,
			false,
			1.f
		);
	}

	if (myExplosionVfx) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, myExplosionVfx, GetActorLocation());
	}
	if (myExplosionSound) {
		UGameplayStatics::PlaySoundAtLocation(this, myExplosionSound, GetActorLocation());
	}
}
