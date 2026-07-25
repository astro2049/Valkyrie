// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInteractionComponent::SetInteractable(AActor* const anInteractableActor)
{
	if (!anInteractableActor) {
		return;
	}

	if (UInteractableComponent* const interactableComponent = anInteractableActor->GetComponentByClass<UInteractableComponent>()) {
		myInteractableComponent = interactableComponent;
	}
}

void UInteractionComponent::ClearInteractable(AActor* const anInteractableActor)
{
	if (!anInteractableActor) {
		return;
	}

	if (myInteractableComponent.IsValid() && myInteractableComponent.Get() == anInteractableActor->GetComponentByClass<UInteractableComponent>()) {
		myInteractableComponent.Reset();
	}
}

void UInteractionComponent::Server_Interact_Implementation()
{
	if (UInteractableComponent* const interactableComponent = myInteractableComponent.Get()) {
		interactableComponent->Interact();
	}
}
