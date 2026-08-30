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
	ActivationOwnedTags.AddTag(AbilityTags::Ability_Reload);
	ActivationBlockedTags.AddTag(AbilityTags::Ability_Reload);
	ActivationBlockedTags.AddTag(AbilityTags::Ability_SwitchWeapon);
}

void UReloadAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AValkPlayerCharacter* const playerCharacter = CastChecked<AValkPlayerCharacter>(ActorInfo->AvatarActor.Get());
	const UWeaponComponent* const weaponComponent = playerCharacter->GetWeaponComponent();
	if (weaponComponent->GetCurrentGunActor() && weaponComponent->CanReload()) {
		UAnimMontage* const reloadMontage = weaponComponent->GetCurrentGunActor()->GetReloadMontage();
		const float montagePlayRate = reloadMontage->GetPlayLength() / weaponComponent->GetReloadDuration();
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

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UReloadAbility::HandleReloadFinished()
{
	const AValkPlayerCharacter* const playerCharacter = CastChecked<AValkPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (playerCharacter->HasAuthority()) {
		playerCharacter->GetWeaponComponent()->ApplyReloadAmmo();
	}
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		playerCharacter->HasAuthority(),
		false
	);
}

void UReloadAbility::HandleReloadCancelled()
{
	const AValkPlayerCharacter* const playerCharacter = CastChecked<AValkPlayerCharacter>(GetAvatarActorFromActorInfo());
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		playerCharacter->HasAuthority(),
		true
	);
}
