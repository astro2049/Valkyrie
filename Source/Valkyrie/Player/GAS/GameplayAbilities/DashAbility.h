// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DashAbility.generated.h"

UCLASS(Abstract, Blueprintable)
class VALKYRIE_API UDashAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDashAbility();

	virtual void ActivateAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	UFUNCTION()
	void HandleDashFinished();

	void EndDash(bool aWasCancelled);

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myDashDistance{600.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myDashDuration{0.2f};
};
