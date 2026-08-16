// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ThrowGrenadeAbility.generated.h"

class AGrenadeActor;
class UAnimMontage;

UCLASS(Abstract, Blueprintable)
class VALKYRIE_API UThrowGrenadeAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UThrowGrenadeAbility();

	virtual void ActivateAbility(
		FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:
	UFUNCTION()
	void HandleGrenadeReleased();
	UFUNCTION()
	void HandleThrowFinished();
	UFUNCTION()
	void HandleThrowCancelled();

	void EndThrow(bool aWasCancelled);

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<AGrenadeActor> myGrenadeActorType{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UAnimMontage> myThrowMontage{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	FName myGrenadeSpawnSocketName{TEXT("hand_r")};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myThrowReleaseTime{0.35f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myThrowSpeed{1200.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myAimDistance{10000.f};
};
