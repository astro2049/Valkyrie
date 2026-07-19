// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AController;

DECLARE_DELEGATE_OneParam(FValkHealthDiedSignature, AController*);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void ApplyDamage(float aDamage, AController* aDamageInstigator);
	void ResetHealth();
	float GetHealth() const { return myHealth; }
	float GetMaxHealth() const { return myMaxHealth; }
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	bool IsDead() const { return myIsDead; }
	FValkHealthDiedSignature& OnDied() { return myOnDied; }

private:
	virtual void BeginPlay() override;

	FValkHealthDiedSignature myOnDied;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myMaxHealth{100.f};
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myHealth{100.f};
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	bool myIsDead{false};
};
