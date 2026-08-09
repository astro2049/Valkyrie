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
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr
	) const override;
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	UFUNCTION()
	void HandleInputReleased(float aTimeHeld);
	UFUNCTION()
	void HandleFireIntervalFinished();
	UFUNCTION()
	void HandleReloadFinished();
	void FireAndWait();
};
