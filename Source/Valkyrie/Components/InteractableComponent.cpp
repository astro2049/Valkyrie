// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractableComponent::Interact() {}
