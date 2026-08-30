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

	const FGameplayAbilitySpec* const abilitySpec = GetCurrentAbilitySpec();
	check(abilitySpec->InputID == EAbilityInputId::PrimaryWeapon || abilitySpec->InputID == EAbilityInputId::SecondaryWeapon);
	myTargetWeaponSlot = abilitySpec->InputID == EAbilityInputId::PrimaryWeapon ? EValkWeaponSlot::Primary : EValkWeaponSlot::Secondary;

	UWeaponComponent* const weaponComponent = CastChecked<AValkPlayerCharacter>(ActorInfo->AvatarActor.Get())->GetWeaponComponent();
	if (weaponComponent->GetCurrentSlot() != myTargetWeaponSlot) {
		weaponComponent->EquipGun(myTargetWeaponSlot);
		const float montagePlayRate = mySwitchWeaponMontage->GetPlayLength() / mySwitchDuration;
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
	}

	EndSwitch(true);
}

void USwitchWeaponAbility::EndSwitch(const bool aWasCancelled)
{
	const AValkPlayerCharacter* const playerCharacter = CastChecked<AValkPlayerCharacter>(GetAvatarActorFromActorInfo());
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		playerCharacter->HasAuthority(),
		aWasCancelled
	);
}
