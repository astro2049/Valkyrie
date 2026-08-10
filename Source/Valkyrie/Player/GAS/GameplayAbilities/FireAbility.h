// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FireAbility.generated.h"

UCLASS(Abstract, Blueprintable)
class VALKYRIE_API UFireAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFireAbility();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	UFUNCTION()
	void HandleInputReleased(float aTimeHeld);
};
