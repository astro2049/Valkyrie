// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapDataAsset.generated.h"

class AGameModeBase;
class UTexture2D;

USTRUCT(BlueprintType)
struct FMenuModeEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Menu|Mode")
	FText myDisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Menu|Mode")
	TSubclassOf<AGameModeBase> myGameModeClass;

	bool IsPlayable() const;
};

UCLASS(BlueprintType)
class VALKYRIE_API UMapDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Menu|Map")
	FText myDisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Menu|Map", meta=(MultiLine=true))
	FText myDescription;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Menu|Map")
	TObjectPtr<UTexture2D> myPreviewImage{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Menu|Map")
	FName myLevelName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Menu|Map")
	TArray<FMenuModeEntry> mySupportedModes;
};
