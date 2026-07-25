// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapDataAsset.generated.h"

class UTexture2D;
class UWorld;

UCLASS(BlueprintType)
class VALKYRIE_API UMapDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valkyrie")
	TObjectPtr<UTexture2D> myMapImage{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valkyrie")
	FText myMapName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valkyrie")
	FText myMapMode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(MultiLine="true"))
	FText myMapDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Valkyrie")
	TSoftObjectPtr<UWorld> myLevel;
};
