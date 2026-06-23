// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FInteractableChanged,
	bool, aHasInteractable
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	UFUNCTION(BlueprintCallable, Category="Valkyrie|Interaction")
	void SetInteractable(AActor* anInteractableActor);
	UFUNCTION(BlueprintCallable, Category="Valkyrie|Interaction")
	void ClearInteractable(AActor* anInteractableActor);

	void TryInteract() const;
	bool HasInteractable() const { return myInteractableComponent.IsValid(); }

	UPROPERTY(BlueprintAssignable, Category="Valkyrie|Interaction")
	FInteractableChanged OnInteractableChanged;

private:
	TWeakObjectPtr<UInteractableComponent> myInteractableComponent;
};
