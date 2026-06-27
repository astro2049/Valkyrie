// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FHealthStateChanged);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FHealthChanged,
	float, aHealth,
	float, aMaxHealth
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category="Health")
	void ApplyDamage(float aDamage);
	UFUNCTION(BlueprintCallable, Category="Health")
	void ResetHealth();

	UFUNCTION(BlueprintPure, Category="Health")
	float GetHealth() const { return myHealth; }
	UFUNCTION(BlueprintPure, Category="Health")
	float GetMaxHealth() const { return myMaxHealth; }
	UFUNCTION(BlueprintPure, Category="Health")
	bool IsDead() const { return myIsDead; }

	FHealthStateChanged myOnHealthStateChanged;
	UPROPERTY(BlueprintAssignable, Category="Health")
	FHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category="Health")
	FDeath OnDeath;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnRep_Health();
	UFUNCTION()
	void OnRep_IsDead();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health", meta=(AllowPrivateAccess="true"))
	float myMaxHealth{100.f};
	UPROPERTY(ReplicatedUsing=OnRep_Health, VisibleAnywhere, BlueprintReadOnly, Category="Health", meta=(AllowPrivateAccess="true"))
	float myHealth{100.f};
	UPROPERTY(ReplicatedUsing=OnRep_IsDead, VisibleAnywhere, BlueprintReadOnly, Category="Health", meta=(AllowPrivateAccess="true"))
	bool myIsDead{false};
};
