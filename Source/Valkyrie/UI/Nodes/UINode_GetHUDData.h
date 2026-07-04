// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UINode_GetHUDData.generated.h"

class APlayerController;

USTRUCT(BlueprintType)
struct FValkHUDData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	float myHealth{0.f};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	float myMaxHealth{0.f};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myAmmo{0};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myReserveAmmo{0};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	bool myIsReloading{false};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	bool myHasInteractable{false};
};

UCLASS()
class VALKYRIE_API UUINode_GetHUDData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static FValkHUDData GetHUDData(APlayerController* aPlayerController);
};
