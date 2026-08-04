// Fill out your copyright notice in the Description page of Project Settings.

#include "AimAbility.h"

#include "Valkyrie/Player/Character/ValkPlayerCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

void UAimAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetAvatarActorFromActorInfo())) {
		playerCharacter->SetAiming(true);

		if (UAbilityTask_WaitInputRelease* const waitTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true)) {
			waitTask->OnRelease.AddDynamic(this, &UAimAbility::HandleInputReleased);
			waitTask->ReadyForActivation();
		}
	}
}
void UAimAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetAvatarActorFromActorInfo())) {
		playerCharacter->SetAiming(false);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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
