// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ValkViewModel.generated.h"

class APlayerController;

UCLASS()
class VALKYRIE_API UValkViewModel : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(APlayerController* aPlayerController);

protected:
	APlayerController* GetPlayerController() const { return myPlayerController.Get(); }

private:
	TWeakObjectPtr<APlayerController> myPlayerController;
};
