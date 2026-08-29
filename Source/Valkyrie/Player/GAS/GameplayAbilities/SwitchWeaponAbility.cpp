// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchWeaponAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/Player/Character/ValkPlayerCharacter.h"
#include "Valkyrie/Player/GAS/AbilityInputId.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

USwitchWeaponAbility::USwitchWeaponAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	FGameplayTagContainer assetTags;
	assetTags.AddTag(AbilityTags::Ability_SwitchWeapon);
	SetAssetTags(assetTags);
	ActivationOwnedTags.AddTag(AbilityTags::Ability_SwitchWeapon);
	CancelAbilitiesWithTag.AddTag(AbilityTags::Ability_SwitchWeapon);
	CancelAbilitiesWithTag.AddTag(AbilityTags::Ability_Aim);
	CancelAbilitiesWithTag.AddTag(AbilityTags::Ability_Fire);
	CancelAbilitiesWithTag.AddTag(AbilityTags::Ability_Reload);
}

void USwitchWeaponAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const FGameplayAbilitySpec* const abilitySpec = GetCurrentAbilitySpec()) {
		if (abilitySpec->InputID == EAbilityInputId::PrimaryWeapon) {
			myTargetWeaponSlot = EValkWeaponSlot::Primary;
		} else if (abilitySpec->InputID == EAbilityInputId::SecondaryWeapon) {
			myTargetWeaponSlot = EValkWeaponSlot::Secondary;
		} else {
			EndSwitch(true);
			return;
		}

		if (const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(ActorInfo->AvatarActor.Get())) {
			if (UWeaponComponent* const weaponComponent = playerCharacter->FindComponentByClass<UWeaponComponent>()) {
				if (weaponComponent->GetCurrentSlot() != myTargetWeaponSlot) {
					weaponComponent->EquipGun(myTargetWeaponSlot);
					if (mySwitchWeaponMontage) {
						const float switchDuration = FMath::Max(mySwitchDuration, UE_KINDA_SMALL_NUMBER);
						const float montagePlayRate = mySwitchWeaponMontage->GetPlayLength() / switchDuration;

						UAbilityTask_PlayMontageAndWait* const montageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
							this,
							NAME_None,
							mySwitchWeaponMontage,
							montagePlayRate
						);
						montageTask->OnCompleted.AddDynamic(this, &USwitchWeaponAbility::HandleSwitchFinished);
						montageTask->OnInterrupted.AddDynamic(this, &USwitchWeaponAbility::HandleSwitchCancelled);
						montageTask->OnCancelled.AddDynamic(this, &USwitchWeaponAbility::HandleSwitchCancelled);
						montageTask->ReadyForActivation();
						return;
					} else {
						HandleSwitchFinished();
						return;
					}
				}
			}
		}
	}

	EndSwitch(true);
}

void USwitchWeaponAbility::EndSwitch(const bool aWasCancelled)
{
	const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetAvatarActorFromActorInfo());
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		playerCharacter && playerCharacter->HasAuthority(),
		aWasCancelled
	);
}
