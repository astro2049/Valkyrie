// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FWeaponHitActor,
	AActor*, HitActor,
	float, Damage
);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UPROPERTY(BlueprintAssignable, Category="Weapon")
	FWeaponHitActor myOnHitActor;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	float myDamage{25.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	float myRPM{600.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	float myTraceDistance{1000.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	USoundBase* myFireSound{nullptr};

	float myLastFiredTime = -1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon|Debug", meta=(AllowPrivateAccess="true"))
	bool myDrawDebugTrace{true};
};
