// Fill out your copyright notice in the Description page of Project Settings.

#include "ReloadAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "Valkyrie/Actors/Gun/GunActor.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/Player/Character/ValkPlayerCharacter.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

UReloadAbility::UReloadAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	FGameplayTagContainer assetTags;
	assetTags.AddTag(AbilityTags::Ability_Reload);
	SetAssetTags(assetTags);
	ActivationOwnedTags.AddTag(AbilityTags::State_Reloading);
	ActivationBlockedTags.AddTag(AbilityTags::State_Reloading);
}

void UReloadAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(ActorInfo->AvatarActor.Get())) {
		if (const UWeaponComponent* const weaponComponent = playerCharacter->FindComponentByClass<UWeaponComponent>(); weaponComponent && weaponComponent->CanReload()) {
			if (const AGunActor* const currentGunActor = weaponComponent->GetCurrentGunActor()) {
				if (UAnimMontage* const reloadMontage = currentGunActor->GetReloadMontage()) {
					float montagePlayRate = 1.f;
					const float reloadDuration = weaponComponent->GetReloadDuration();
					if (reloadDuration > 0.f) {
						montagePlayRate = reloadMontage->GetPlayLength() / reloadDuration;
					}

					UAbilityTask_PlayMontageAndWait* const montageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
						this,
						NAME_None,
						reloadMontage,
						montagePlayRate
					);
					montageTask->OnCompleted.AddDynamic(this, &UReloadAbility::HandleReloadFinished);
					montageTask->OnInterrupted.AddDynamic(this, &UReloadAbility::HandleReloadCancelled);
					montageTask->OnCancelled.AddDynamic(this, &UReloadAbility::HandleReloadCancelled);
					montageTask->ReadyForActivation();

					return;
				}
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UReloadAbility::HandleReloadFinished()
{
	const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (playerCharacter && playerCharacter->HasAuthority()) {
		if (UWeaponComponent* const weaponComponent = playerCharacter->FindComponentByClass<UWeaponComponent>()) {
			weaponComponent->ApplyReloadAmmo();
		}
	}
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		playerCharacter && playerCharacter->HasAuthority(),
		false
	);
}

void UReloadAbility::HandleReloadCancelled()
{
	const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetAvatarActorFromActorInfo());
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		playerCharacter && playerCharacter->HasAuthority(),
		true
	);
}
