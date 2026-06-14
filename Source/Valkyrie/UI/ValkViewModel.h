// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ValkViewModel.generated.h"

DECLARE_MULTICAST_DELEGATE(FValkViewModelChanged);

UCLASS()
class VALKYRIE_API UValkViewModel : public UObject
{
	GENERATED_BODY()

public:
	FValkViewModelChanged myOnViewModelChanged;

protected:
	void BroadcastViewModelChanged();
};
