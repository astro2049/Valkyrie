// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtractionGenerator.generated.h"

class UGeneratorInteractableComponent;

UCLASS(Blueprintable)
class VALKYRIE_API AExtractionGenerator : public AActor
{
	GENERATED_BODY()

public:
	AExtractionGenerator();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UGeneratorInteractableComponent> myInteractableComponent;
};
