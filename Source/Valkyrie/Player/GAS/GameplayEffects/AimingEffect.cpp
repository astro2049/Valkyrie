// Fill out your copyright notice in the Description page of Project Settings.

#include "AimingEffect.h"

#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

UAimingEffect::UAimingEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	FInheritedTagContainer tagChanges;
	tagChanges.Added.AddTag(AbilityTags::State_Aiming);

	UTargetTagsGameplayEffectComponent* const targetTagsComponent = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTagsComponent"));
	GEComponents.Add(targetTagsComponent);
	targetTagsComponent->SetAndApplyTargetTagChanges(tagChanges);
}
