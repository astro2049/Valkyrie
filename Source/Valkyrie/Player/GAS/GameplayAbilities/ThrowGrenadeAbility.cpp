// Fill out your copyright notice in the Description page of Project Settings.

#include "ThrowGrenadeAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Valkyrie/Actors/Grenade/GrenadeActor.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"

UThrowGrenadeAbility::UThrowGrenadeAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	FGameplayTagContainer assetTags;
	assetTags.AddTag(AbilityTags::Ability_ThrowGrenade);
	SetAssetTags(assetTags);
	ActivationOwnedTags.AddTag(AbilityTags::Ability_ThrowGrenade);
	ActivationBlockedTags.AddTag(AbilityTags::Ability_ThrowGrenade);
}

void UThrowGrenadeAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ACharacter* const character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (character && myGrenadeActorType && myThrowMontage
		&& myThrowReleaseTime >= 0.f && myThrowReleaseTime <= myThrowMontage->GetPlayLength()
		&& myThrowSpeed > 0.f && myAimDistance > 0.f) {
		UAbilityTask_PlayMontageAndWait* const montageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			myThrowMontage
		);
		montageTask->OnCompleted.AddDynamic(this, &UThrowGrenadeAbility::HandleThrowFinished);
		montageTask->OnInterrupted.AddDynamic(this, &UThrowGrenadeAbility::HandleThrowCancelled);
		montageTask->OnCancelled.AddDynamic(this, &UThrowGrenadeAbility::HandleThrowCancelled);
		montageTask->ReadyForActivation();

		UAbilityTask_WaitDelay* const releaseTask = UAbilityTask_WaitDelay::WaitDelay(this, myThrowReleaseTime);
		releaseTask->OnFinish.AddDynamic(this, &UThrowGrenadeAbility::HandleGrenadeReleased);
		releaseTask->ReadyForActivation();
		return;
	}

	EndThrow(true);
}

void UThrowGrenadeAbility::HandleGrenadeReleased()
{
	ACharacter* const character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	UWorld* const world = GetWorld();
	if (character && character->HasAuthority() && world) {
		FVector spawnLocation = character->GetActorLocation()
			+ character->GetActorForwardVector() * 50.f
			+ FVector::UpVector * 50.f;
		if (const USkeletalMeshComponent* const characterMesh = character->GetMesh(); characterMesh && characterMesh->DoesSocketExist(myGrenadeSpawnSocketName)) {
			spawnLocation = characterMesh->GetSocketLocation(myGrenadeSpawnSocketName);
		}

		FVector viewLocation = spawnLocation;
		FRotator viewRotation = character->GetControlRotation();
		if (const AController* const controller = character->GetController()) {
			controller->GetPlayerViewPoint(viewLocation, viewRotation);
		}
		const FVector aimPoint = viewLocation + viewRotation.Vector() * myAimDistance;
		const FVector throwDirection = (aimPoint - spawnLocation).GetSafeNormal();

		FActorSpawnParameters spawnParams;
		spawnParams.Owner = character;
		spawnParams.Instigator = character;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AGrenadeActor* const grenadeActor = world->SpawnActor<AGrenadeActor>(
			myGrenadeActorType,
			spawnLocation,
			throwDirection.Rotation(),
			spawnParams
		)) {
			grenadeActor->Launch(throwDirection, myThrowSpeed);
		}
	}
}

void UThrowGrenadeAbility::EndThrow(const bool aWasCancelled)
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
