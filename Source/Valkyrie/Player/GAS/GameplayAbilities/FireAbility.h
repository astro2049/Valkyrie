// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FireAbility.generated.h"

class UWeaponComponent;

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
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

private:
	void FireStep();
	UFUNCTION()
	void HandleInputReleased(float aTimeHeld);

	UPROPERTY()
	TObjectPtr<UWeaponComponent> myWeaponComponent{nullptr};
	FTimerHandle myFireTimerHandle;
};
