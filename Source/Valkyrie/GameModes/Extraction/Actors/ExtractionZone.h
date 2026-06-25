// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtractionZone.generated.h"

UCLASS(Blueprintable)
class VALKYRIE_API AExtractionZone : public AActor
{
	GENERATED_BODY()

public:
	AExtractionZone();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ActivateForAll();

	UFUNCTION(BlueprintImplementableEvent, Category="Valkyrie|Extraction")
	void Activate();

	UFUNCTION(BlueprintCallable, Category="Valkyrie|Extraction")
	void HandlePlayerEntered(AActor* anOtherActor);

private:
	UFUNCTION()
	void OnRep_IsActive();

	UPROPERTY(ReplicatedUsing=OnRep_IsActive)
	bool myIsActive{false};
};
