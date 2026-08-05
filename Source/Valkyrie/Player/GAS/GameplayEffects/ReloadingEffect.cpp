// Fill out your copyright notice in the Description page of Project Settings.

#include "ReloadingEffect.h"

#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

UReloadingEffect::UReloadingEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	FInheritedTagContainer tagChanges;
	tagChanges.Added.AddTag(AbilityTags::State_Reloading);

	UTargetTagsGameplayEffectComponent* const targetTagsComponent = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>(TEXT("TargetTagsComponent"));
	GEComponents.Add(targetTagsComponent);
	targetTagsComponent->SetAndApplyTargetTagChanges(tagChanges);
}
