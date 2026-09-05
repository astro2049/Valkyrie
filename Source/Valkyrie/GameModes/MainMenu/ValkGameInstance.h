// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ValkGameInstance.generated.h"

class UDataAsset_Map;

UCLASS()
class VALKYRIE_API UValkGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void SetSelectedMapDataAsset(UDataAsset_Map* const aMapDataAsset) { mySelectedMap = aMapDataAsset; }
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	UDataAsset_Map* GetSelectedMapDataAsset() const { return mySelectedMap; }

private:
	UPROPERTY()
	TObjectPtr<UDataAsset_Map> mySelectedMap{nullptr};
};
