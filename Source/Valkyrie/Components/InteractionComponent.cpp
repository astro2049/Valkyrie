// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::SetInteractable(AActor* anInteractableActor)
{
	if (anInteractableActor) {
		if (UInteractableComponent* interactable =
			anInteractableActor->GetComponentByClass<UInteractableComponent>()) {
			myInteractableComponent = interactable;
		}
	}
}

void UInteractionComponent::ClearInteractable(AActor* anInteractableActor)
{
	if (anInteractableActor) {
		if (myInteractableComponent.Get() ==
			anInteractableActor->GetComponentByClass<UInteractableComponent>()) {
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
