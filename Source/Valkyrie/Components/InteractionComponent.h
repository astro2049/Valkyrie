// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FInteractionStateChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void SetInteractable(AActor* anInteractableActor);
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void ClearInteractable(AActor* anInteractableActor);
	void Interact() const;
	
	bool HasInteractable() const { return myInteractableComponent.IsValid(); }

	FInteractionStateChanged myOnInteractionStateChanged;

private:
	TWeakObjectPtr<UInteractableComponent> myInteractableComponent;
};
