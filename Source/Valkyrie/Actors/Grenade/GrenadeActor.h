// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeActor.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USoundBase;
class USphereComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class VALKYRIE_API AGrenadeActor : public AActor
{
	GENERATED_BODY()

public:
	AGrenadeActor();

	void Launch(FVector aDirection, float aSpeed);

private:
	virtual void BeginPlay() override;

	void Explode();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayExplosionPresentation();

	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<USphereComponent> myCollisionComponent;
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UStaticMeshComponent> myMeshComponent;
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UProjectileMovementComponent> myProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myFuseDuration{3.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myExplosionRadius{400.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myDamage{75.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UNiagaraSystem> myExplosionVfx{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<USoundBase> myExplosionSound{nullptr};

	FTimerHandle myFuseTimerHandle;
};
