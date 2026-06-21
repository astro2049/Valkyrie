// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


void UInteractionComponent::HandleBeginOverlap(UPrimitiveComponent* anOverlappedComponent,
                                               AActor* anOtherActor,
                                               UPrimitiveComponent* anOtherComponent,
                                               int32 anOtherBodyIndex,
                                               bool aFromSweep,
                                               const FHitResult& aSweepResult)
{
	if (anOtherActor) {
		if (UInteractableComponent* interactable = anOtherActor->GetComponentByClass<UInteractableComponent>()) {
			myInteractableComponent = interactable;
		}
	}
}

void UInteractionComponent::HandleEndOverlap(UPrimitiveComponent* anOverlappedComponent,
                                             AActor* anOtherActor,
                                             UPrimitiveComponent* anOtherComponent,
                                             int32 anOtherBodyIndex)
{
	if (anOtherActor) {
		if (myInteractableComponent.Get() == anOtherActor->GetComponentByClass<UInteractableComponent>()) {
			myInteractableComponent.Reset();
		}
	}
}

void UInteractionComponent::TryInteract() const
{
	if (UInteractableComponent* interactable = myInteractableComponent.Get()) {
		interactable->Interact();
	}
}
