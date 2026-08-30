// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AController;

DECLARE_DELEGATE_TwoParams(FHealthComponentDamaged, float, AController*);
DECLARE_DELEGATE_OneParam(FHealthComponentDied, AController*);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Reset();
	void ApplyDamage(float aDamage, AController* anAttacker);

	FHealthComponentDamaged& GetOnDamaged() { return myDamagedDelegate; }
	FHealthComponentDied& GetOnDied() { return myDiedDelegate; }

	UFUNCTION(BlueprintPure, Category="Valkyrie")
	float GetMaxHealth() const { return myMaxHealth; }
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	float GetHealth() const { return myHealth; }
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	bool GetIsDead() const { return myIsDead; }

private:
	virtual void BeginPlay() override;

	FHealthComponentDamaged myDamagedDelegate;
	FHealthComponentDied myDiedDelegate;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myMaxHealth{100.f};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	float myHealth{100.f};
	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	bool myIsDead{false};
};
