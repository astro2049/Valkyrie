// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "SwitchWeaponAbility.generated.h"

class UAnimMontage;

UCLASS(Abstract, Blueprintable)
class VALKYRIE_API USwitchWeaponAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USwitchWeaponAbility();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	void PlayUnholsterMontage();
	void EndSwitch(bool aWasCancelled);

	UFUNCTION()
	void HandleHolsterFinished();
	UFUNCTION()
	void HandleUnholsterFinished();
	UFUNCTION()
	void HandleSwitchCancelled();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UAnimMontage> myHolsterMontage{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UAnimMontage> myUnholsterMontage{nullptr};

	EValkWeaponSlot myTargetWeaponSlot{EValkWeaponSlot::Primary};
};
