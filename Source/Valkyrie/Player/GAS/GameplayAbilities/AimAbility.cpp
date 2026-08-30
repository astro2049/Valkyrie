// Fill out your copyright notice in the Description page of Project Settings.

#include "AimAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

UAimAbility::UAimAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	FGameplayTagContainer assetTags;
	assetTags.AddTag(AbilityTags::Ability_Aim);
	SetAssetTags(assetTags);
	ActivationOwnedTags.AddTag(AbilityTags::Ability_Aim);
	ActivationBlockedTags.AddTag(AbilityTags::Ability_SwitchWeapon);
}

void UAimAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitInputRelease* const waitTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	waitTask->OnRelease.AddDynamic(this, &UAimAbility::HandleInputReleased);
	waitTask->ReadyForActivation();
}

void UAimAbility::HandleInputReleased(float aTimeHeld)
{
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		true,
		false
	);
}
