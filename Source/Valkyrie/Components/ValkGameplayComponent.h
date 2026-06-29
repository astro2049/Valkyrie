// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ValkGameplayComponent.generated.h"

enum class EUIMessageType : uint8;

UCLASS(Abstract)
class VALKYRIE_API UValkGameplayComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	void BroadcastLocalPlayerUIMessage(EUIMessageType aMessageType) const;
};
