// Fill out your copyright notice in the Description page of Project Settings.

#include "FireAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Engine/World.h"
#include "TimerManager.h"
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

	myWeaponComponent = ActorInfo && ActorInfo->AvatarActor.IsValid()
		? ActorInfo->AvatarActor->FindComponentByClass<UWeaponComponent>()
		: nullptr;
	UWorld* const world = GetWorld();
	const float fireInterval = myWeaponComponent ? myWeaponComponent->GetFireInterval() : 0.f;
	if (myWeaponComponent && world && fireInterval > 0.f) {
		if (UAbilityTask_WaitInputRelease* const inputTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true)) {
			inputTask->OnRelease.AddDynamic(this, &UFireAbility::HandleInputReleased);
			inputTask->ReadyForActivation();
		}

		FireStep();
		world->GetTimerManager().SetTimer(
			myFireTimerHandle,
			this,
			&UFireAbility::FireStep,
			fireInterval,
			true
		);
	} else {
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UFireAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateEndAbility,
	const bool bWasCancelled)
{
	if (UWorld* const world = GetWorld()) {
		world->GetTimerManager().ClearTimer(myFireTimerHandle);
	}
	myWeaponComponent = nullptr;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UFireAbility::FireStep()
{
	if (const FGameplayAbilityActorInfo* const actorInfo = GetCurrentActorInfo(); actorInfo && myWeaponComponent) {
		if (actorInfo->IsLocallyControlled()) {
			myWeaponComponent->PlayPredictedFire();
		}
		if (actorInfo->IsNetAuthority()) {
			myWeaponComponent->TryCommitFire();
		}
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
