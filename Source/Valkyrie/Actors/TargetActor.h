// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "TargetActor.generated.h"

class AController;
class UHealthComponent;

UCLASS(Blueprintable)
class VALKYRIE_API ATargetActor : public AActor
{
	GENERATED_BODY()

public:
	ATargetActor();

private:
	virtual void BeginPlay() override;

	void OnHealthDied(AController* anAttacker);
	void Respawn();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myRespawnDelay{2.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UHealthComponent> myHealthComponent{nullptr};
	FTimerHandle myRespawnTimerHandle;
};
