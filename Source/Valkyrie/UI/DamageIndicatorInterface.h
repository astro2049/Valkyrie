// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageIndicatorInterface.generated.h"

UINTERFACE(Blueprintable)
class VALKYRIE_API UDamageIndicatorInterface : public UInterface
{
	GENERATED_BODY()
};

class VALKYRIE_API IDamageIndicatorInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Valkyrie")
	void PlayDamageIndicator(FVector aDamageSourceLocation);
};
