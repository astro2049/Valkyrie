// Fill out your copyright notice in the Description page of Project Settings.


#include "UIMessageSubsystem.h"

void UUIMessageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FWorldDelegates::OnWorldPreActorTick.AddUObject(
		this,
		&UUIMessageSubsystem::HandlePreActorTick
	);
}

bool UUIMessageSubsystem::CheckUIMessage(UIMessage aUIMessage) const
{
	return myMessagesLastFrame[static_cast<uint8>(aUIMessage)];
}

void UUIMessageSubsystem::BroadcastUIMessage(UIMessage aUIMessage)
{
	myMessagesThisFrame[static_cast<uint8>(aUIMessage)] = true;
}

void UUIMessageSubsystem::HandlePreActorTick(UWorld* aWorld, ELevelTick aLevelTick, float aDeltaTime)
{
	myMessagesLastFrame = myMessagesThisFrame;
	for (int i = 0; i < myMessagesThisFrame.Num(); i++) {
		myMessagesThisFrame[i] = false;
	}
}
