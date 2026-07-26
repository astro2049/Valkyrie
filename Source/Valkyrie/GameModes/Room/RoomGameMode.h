// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/ValkGameMode.h"
#include "RoomGameMode.generated.h"

UCLASS()
class VALKYRIE_API ARoomGameMode : public AValkGameMode
{
	GENERATED_BODY()

public:
	ARoomGameMode();
	virtual void InitGame(const FString& aMapName, const FString& someOptions, FString& anErrorMessage) override;
	
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void StartGame(TSoftObjectPtr<UWorld> aLevel) const;
};
