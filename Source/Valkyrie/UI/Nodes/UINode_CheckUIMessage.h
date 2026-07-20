// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Valkyrie/UI/UIMessageSubsystem.h"
#include "UINode_CheckUIMessage.generated.h"

UCLASS()
class VALKYRIE_API UUINode_CheckUIMessage : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie", meta=(WorldContext="aWorldContextObject"))
	static bool CheckUIMessage(const UObject* aWorldContextObject, UIMessage aUIMessage);
};
