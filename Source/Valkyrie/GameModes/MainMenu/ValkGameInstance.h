// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ValkGameInstance.generated.h"

class UMapDataAsset;

UCLASS()
class VALKYRIE_API UValkGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void SetSelectedMapDataAsset(UMapDataAsset* const aMapDataAsset) { mySelectedMap = aMapDataAsset; }
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	UMapDataAsset* GetSelectedMapDataAsset() const { return mySelectedMap; }

private:
	UPROPERTY()
	TObjectPtr<UMapDataAsset> mySelectedMap{nullptr};
};
