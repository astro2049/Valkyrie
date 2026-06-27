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
		if (UInteractableComponent* interactable = anInteractableActor->GetComponentByClass<UInteractableComponent>()) {
			UE_LOG(LogTemp, Log, TEXT("UInteractionComponent::SetInteractable"));
			myInteractableComponent = interactable;
			myOnInteractionStateChanged.Broadcast();
		}
	}
}

void UInteractionComponent::ClearInteractable(AActor* anInteractableActor)
{
	if (anInteractableActor) {
		if (myInteractableComponent.Get() == anInteractableActor->GetComponentByClass<UInteractableComponent>()) {
			UE_LOG(LogTemp, Log, TEXT("UInteractionComponent::ClearInteractable"));
			myInteractableComponent.Reset();
			myOnInteractionStateChanged.Broadcast();
		}
	}
}

void UInteractionComponent::TryInteract() const
{
	if (UInteractableComponent* interactable = myInteractableComponent.Get()) {
		UE_LOG(LogTemp, Log, TEXT("UInteractionComponent::TryInteract"));
		interactable->Interact();
	}
}
