// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Components/InteractableComponent.h"
#include "GeneratorInteractableComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UGeneratorInteractableComponent : public UInteractableComponent
{
	GENERATED_BODY()

public:
	virtual void Interact() override;
};
