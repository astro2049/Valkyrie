// Fill out your copyright notice in the Description page of Project Settings.

#include "ReloadAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimMontage.h"
#include "Valkyrie/Actors/Gun/GunActor.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/Player/Character/ValkPlayerCharacter.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"
#include "Valkyrie/Player/GAS/GameplayEffects/ReloadingEffect.h"

UReloadAbility::UReloadAbility()
{
	FGameplayTagContainer assetTags;
	assetTags.AddTag(AbilityTags::Ability_Reload);
	SetAssetTags(assetTags);

	ActivationBlockedTags.AddTag(AbilityTags::State_Reloading);
}

void UReloadAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(ActorInfo->AvatarActor.Get());
	UWeaponComponent* const weaponComponent = playerCharacter
		? playerCharacter->FindComponentByClass<UWeaponComponent>()
		: nullptr;
	if (!weaponComponent || !weaponComponent->CanReload()) {
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	AGunActor* const currentGunActor = weaponComponent->GetCurrentGunActor();
	UAnimMontage* const reloadMontage = currentGunActor
		? currentGunActor->GetReloadMontage()
		: nullptr;
	if (!reloadMontage) {
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	myReloadingEffectHandle = ApplyGameplayEffectToOwner(
		Handle,
		ActorInfo,
		ActivationInfo,
		GetDefault<UReloadingEffect>(),
		1.f
	);

	float montagePlayRate = 1.f;
	const float reloadDuration = weaponComponent->GetReloadDuration();
	if (reloadDuration > 0.f) {
		montagePlayRate = reloadMontage->GetPlayLength() / reloadDuration;
	}
	UAbilityTask_PlayMontageAndWait* const montageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			reloadMontage,
			montagePlayRate
		);
	montageTask->OnCompleted.AddDynamic(this, &UReloadAbility::HandleReloadFinished);
	montageTask->OnInterrupted.AddDynamic(this, &UReloadAbility::HandleReloadCancelled);
	montageTask->OnCancelled.AddDynamic(this, &UReloadAbility::HandleReloadCancelled);
	montageTask->ReadyForActivation();
}

void UReloadAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(myReloadingEffectHandle);
	myReloadingEffectHandle.Invalidate();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UReloadAbility::HandleReloadFinished()
{
	AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetAvatarActorFromActorInfo());
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
	if (!IsActive()) {
		return;
	}

	const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(GetAvatarActorFromActorInfo());
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		playerCharacter && playerCharacter->HasAuthority(),
		true
	);
}
