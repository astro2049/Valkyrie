// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtractionZone.generated.h"

class UBoxComponent;

UCLASS(Blueprintable)
class VALKYRIE_API AExtractionZone : public AActor
{
	GENERATED_BODY()

public:
	AExtractionZone();

	void Activate();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleBeginOverlap(
		UPrimitiveComponent* anOverlappedComponent,
		AActor* anOtherActor,
		UPrimitiveComponent* anOtherComponent,
		int32 anOtherBodyIndex,
		bool aFromSweep,
		const FHitResult& aSweepResult
	);

	UPROPERTY(Transient)
	TObjectPtr<UBoxComponent> myBoxComponent;
};
