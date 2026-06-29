// Fill out your copyright notice in the Description page of Project Settings.

#include "UIMessageSubsystem.h"

#include "Engine/World.h"

void UUIMessageSubsystem::Initialize(FSubsystemCollectionBase& aCollection)
{
	Super::Initialize(aCollection);

	myWorldPreActorTickHandle = FWorldDelegates::OnWorldPreActorTick.AddUObject(
		this,
		&UUIMessageSubsystem::HandleWorldPreActorTick
	);
}

void UUIMessageSubsystem::Deinitialize()
{
	FWorldDelegates::OnWorldPreActorTick.Remove(myWorldPreActorTickHandle);
	myWorldPreActorTickHandle.Reset();

	Super::Deinitialize();
}

bool UUIMessageSubsystem::CheckUIMessage(const EUIMessageType aMessageType) const
{
	return myReadMessages[static_cast<int32>(aMessageType)];
}

void UUIMessageSubsystem::BroadcastUIMessage(const EUIMessageType aMessageType)
{
	myWriteMessages[static_cast<int32>(aMessageType)] = true;
}

void UUIMessageSubsystem::HandleWorldPreActorTick(
	UWorld* const aWorld,
	const ELevelTick,
	const float
)
{
	if (aWorld == GetWorld()) {
		Swap(myReadMessages, myWriteMessages);
		for (bool& message : myWriteMessages) {
			message = false;
		}
	}
}
