// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AController;

DECLARE_MULTICAST_DELEGATE_OneParam(FHealthDeath, AController*);
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
	
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void ApplyDamage(float aDamage, AController* aDamageInstigator = nullptr);
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void ResetHealth();
	float GetHealth() const { return myHealth; }
	float GetMaxHealth() const { return myMaxHealth; }
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	bool IsDead() const { return myIsDead; }

	FHealthDeath myOnHealthDeath; // for authoritative game mode rules
	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FHealthChanged OnHealthChanged; // for health bar presenter component (Blueprint)
	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FDeath OnDeath; // for player controller

private:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_Health() const;
	UFUNCTION()
	void OnRep_IsDead() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myMaxHealth{100.f};
	UPROPERTY(ReplicatedUsing=OnRep_Health, VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myHealth{100.f};
	UPROPERTY(ReplicatedUsing=OnRep_IsDead, VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	bool myIsDead{false};
};
