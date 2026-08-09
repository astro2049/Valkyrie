// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ReloadAbility.generated.h"

UCLASS(Abstract, Blueprintable)
class VALKYRIE_API UReloadAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UReloadAbility();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	UFUNCTION()
	void HandleReloadFinished();
	UFUNCTION()
	void HandleReloadCancelled();
};
