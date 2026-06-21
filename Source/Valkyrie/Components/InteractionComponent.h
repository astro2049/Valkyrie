// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionComponent();

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* anOverlappedComponent,
	                        AActor* anOtherActor,
	                        UPrimitiveComponent* anOtherComponent,
	                        int32 anOtherBodyIndex,
	                        bool aFromSweep,
	                        const FHitResult& aSweepResult);
	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* anOverlappedComponent,
	                      AActor* anOtherActor,
	                      UPrimitiveComponent* anOtherComponent,
	                      int32 anOtherBodyIndex);
	void TryInteract() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<UInteractableComponent> myInteractableComponent;
};
