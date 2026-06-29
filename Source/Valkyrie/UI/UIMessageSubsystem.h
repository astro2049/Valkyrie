// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/StaticArray.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIMessageSubsystem.generated.h"

#define VALK_UISUBSYS() \
	(GetWorld() ? GetWorld()->GetSubsystem<UUIMessageSubsystem>() : nullptr)

UENUM()
enum class EUIMessageType : uint8
{
	LocalPawnStateUpdated,
	GameStateUpdated,
	PlayerStateUpdated,
	Count
};

UCLASS()
class VALKYRIE_API UUIMessageSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& aCollection) override;
	virtual void Deinitialize() override;

	void BroadcastUIMessage(EUIMessageType aMessageType);
	bool CheckUIMessage(EUIMessageType aMessageType) const;

private:
	void HandleWorldPreActorTick(UWorld* aWorld, ELevelTick aTickType, float aDeltaSeconds);

	TStaticArray<bool, static_cast<uint32>(EUIMessageType::Count)> myReadMessages{};
	TStaticArray<bool, static_cast<uint32>(EUIMessageType::Count)> myWriteMessages{};
	FDelegateHandle myWorldPreActorTickHandle;
};
