// Fill out your copyright notice in the Description page of Project Settings.

#include "DashAbility.h"

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

UDashAbility::UDashAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	FGameplayTagContainer assetTags;
	assetTags.AddTag(AbilityTags::Ability_Dash);
	SetAssetTags(assetTags);
}

void UDashAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ACharacter* const character = Cast<ACharacter>(ActorInfo->AvatarActor.Get())) {
		const FVector dashDirection = character->GetActorForwardVector().GetSafeNormal2D();
		if (!dashDirection.IsNearlyZero() && myDashDistance > 0.f && myDashDuration > 0.f) {
			if (CommitAbility(Handle, ActorInfo, ActivationInfo)) {
				UAbilityTask_ApplyRootMotionConstantForce* const dashTask =
					UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
						this,
						NAME_None,
						dashDirection,
						myDashDistance / myDashDuration,
						myDashDuration,
						false,
						nullptr,
						ERootMotionFinishVelocityMode::ClampVelocity,
						FVector::ZeroVector,
						character->GetCharacterMovement()->MaxWalkSpeed,
						true
					);
				dashTask->OnFinish.AddDynamic(this, &UDashAbility::HandleDashFinished);
				dashTask->ReadyForActivation();
				return;
			}
		}
	}

	EndDash(true);
}

void UDashAbility::HandleDashFinished()
{
	EndDash(false);
}

void UDashAbility::EndDash(const bool aWasCancelled)
{
	const ACharacter* const character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	EndAbility(
		GetCurrentAbilitySpecHandle(),
		GetCurrentActorInfo(),
		GetCurrentActivationInfo(),
		character && character->HasAuthority(),
		aWasCancelled
	);
}
