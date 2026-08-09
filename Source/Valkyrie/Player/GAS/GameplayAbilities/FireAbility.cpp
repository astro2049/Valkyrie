// Fill out your copyright notice in the Description page of Project Settings.

#include "FireAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/Player/Character/ValkPlayerCharacter.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

UFireAbility::UFireAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UFireAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) {
		return false;
	}

	const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(ActorInfo->AvatarActor.Get());
	const UWeaponComponent* const weaponComponent = playerCharacter
		? playerCharacter->FindComponentByClass<UWeaponComponent>()
		: nullptr;
	return weaponComponent && (weaponComponent->CanFire() || weaponComponent->IsReloading());
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
	const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (playerCharacter && playerCharacter->IsLocallyControlled()) {
		FireAndWait();
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

void UFireAbility::HandleFireIntervalFinished()
{
	if (IsActive()) {
		FireAndWait();
	}
}

void UFireAbility::HandleReloadFinished()
{
	if (IsActive()) {
		FireAndWait();
	}
}

void UFireAbility::FireAndWait()
{
	AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetAvatarActorFromActorInfo());
	UWeaponComponent* const weaponComponent = playerCharacter
		? playerCharacter->FindComponentByClass<UWeaponComponent>()
		: nullptr;
	if (!weaponComponent) {
		EndAbility(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			true,
			false
		);
		return;
	}
	if (weaponComponent->IsReloading()) {
		if (UAbilityTask_WaitGameplayTagRemoved* const reloadTask =
			UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(
				this,
				AbilityTags::State_Reloading,
				nullptr,
				true
			)) {
			reloadTask->Removed.AddDynamic(this, &UFireAbility::HandleReloadFinished);
			reloadTask->ReadyForActivation();
		}
		return;
	}
	if (!weaponComponent->CanFire()) {
		EndAbility(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			true,
			false
		);
		return;
	}

	weaponComponent->Fire();

	const float fireInterval = weaponComponent->GetFireInterval();
	if (fireInterval <= 0.f) {
		EndAbility(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			true,
			false
		);
		return;
	}

	if (UAbilityTask_WaitDelay* const fireIntervalTask = UAbilityTask_WaitDelay::WaitDelay(this, fireInterval)) {
		fireIntervalTask->OnFinish.AddDynamic(this, &UFireAbility::HandleFireIntervalFinished);
		fireIntervalTask->ReadyForActivation();
	}
}
