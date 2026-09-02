// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RoomGameMode.generated.h"

UCLASS()
class VALKYRIE_API ARoomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARoomGameMode();
	virtual void PostLogin(APlayerController* aNewPlayer) override;
	
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void StartGame(TSoftObjectPtr<UWorld> aLevel) const;
};
