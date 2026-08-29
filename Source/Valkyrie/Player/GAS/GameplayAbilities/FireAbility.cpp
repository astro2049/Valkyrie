// Fill out your copyright notice in the Description page of Project Settings.

#include "FireAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

UFireAbility::UFireAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	FGameplayTagContainer assetTags;
	assetTags.AddTag(AbilityTags::Ability_Fire);
	SetAssetTags(assetTags);
	ActivationOwnedTags.AddTag(AbilityTags::Ability_Fire);
	ActivationBlockedTags.AddTag(AbilityTags::Ability_SwitchWeapon);
}

void UFireAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ActorInfo->AvatarActor->FindComponentByClass<UWeaponComponent>()->StartFiring();
	UAbilityTask_WaitInputRelease* const waitTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	waitTask->OnRelease.AddDynamic(this, &UFireAbility::HandleInputReleased);
	waitTask->ReadyForActivation();
}

void UFireAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateEndAbility,
	const bool bWasCancelled)
{
	ActorInfo->AvatarActor->FindComponentByClass<UWeaponComponent>()->StopFiring();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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
