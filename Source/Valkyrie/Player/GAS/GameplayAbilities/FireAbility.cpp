// Fill out your copyright notice in the Description page of Project Settings.

#include "FireAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

UFireAbility::UFireAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ActivationOwnedTags.AddTag(AbilityTags::Ability_Fire);
}

void UFireAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilityTask_WaitInputRelease* const inputTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true)) {
		inputTask->OnRelease.AddDynamic(this, &UFireAbility::HandleInputReleased);
		inputTask->ReadyForActivation();
	}
}

void UFireAbility::HandleInputReleased(const float aTimeHeld)
{
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		true,
		false
	);
}
