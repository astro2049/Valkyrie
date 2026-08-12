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
	void EndSwitch(bool aWasCancelled);

	UFUNCTION()
	void HandleSwitchFinished();
	UFUNCTION()
	void HandleSwitchCancelled();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UAnimMontage> mySwitchWeaponMontage{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float mySwitchDuration{0.3f};

	EValkWeaponSlot myTargetWeaponSlot{EValkWeaponSlot::Primary};
};
