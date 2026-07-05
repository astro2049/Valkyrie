// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::SetInteractable(AActor* const anInteractableActor)
{
	if (!anInteractableActor) {
		return;
	}

	if (UInteractableComponent* const interactableComponent = anInteractableActor->GetComponentByClass<UInteractableComponent>()) {
		UE_LOG(LogTemp, Log, TEXT("UInteractionComponent::SetInteractable"));
		myInteractableComponent = interactableComponent;
	}
}

void UInteractionComponent::ClearInteractable(AActor* const anInteractableActor)
{
	if (!anInteractableActor) {
		return;
	}

	if (myInteractableComponent.IsValid()) {
		if (myInteractableComponent.Get() == anInteractableActor->GetComponentByClass<UInteractableComponent>()) {
			UE_LOG(LogTemp, Log, TEXT("UInteractionComponent::ClearInteractable"));
			myInteractableComponent.Reset();
		}
	}
}

void UInteractionComponent::Interact() const
{
	if (UInteractableComponent* const interactableComponent = myInteractableComponent.Get()) {
		UE_LOG(LogTemp, Log, TEXT("UInteractionComponent::Interact"));
		interactableComponent->Interact();
	}
}
