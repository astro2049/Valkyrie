// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIMessageSubsystem.generated.h"

#define VALK_UIMESSAGESUBSYS() \
	GetWorld() ? GetWorld()->GetSubsystem<UUIMessageSubsystem>() : nullptr

/**
 * 
 */

UENUM(BlueprintType)
enum class UIMessage : uint8
{
	LocalPlayerHitConfirmed = 0,

	Count
};

UCLASS()
class VALKYRIE_API UUIMessageSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	bool CheckUIMessage(UIMessage aUIMessage) const;
	void BroadcastUIMessage(UIMessage aUIMessage);

private:
	void HandlePreActorTick(UWorld* aWorld, ELevelTick aLevelTick, float aDeltaTime);

	TStaticArray<bool, static_cast<uint8>(UIMessage::Count)> myMessagesLastFrame;
	TStaticArray<bool, static_cast<uint8>(UIMessage::Count)> myMessagesThisFrame;
};
