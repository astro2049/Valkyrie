// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionGenerator.h"

#include "GeneratorInteractableComponent.h"

AExtractionGenerator::AExtractionGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
	myInteractableComponent = CreateDefaultSubobject<UGeneratorInteractableComponent>(TEXT("InteractableComponent"));
}
