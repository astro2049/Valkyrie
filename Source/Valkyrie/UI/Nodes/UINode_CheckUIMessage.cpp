// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_CheckUIMessage.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

bool UUINode_CheckUIMessage::CheckUIMessage(const UObject* const aWorldContextObject, const UIMessage aUIMessage)
{
	if (!aWorldContextObject) {
		return false;
	}

	if (const UWorld* const world = aWorldContextObject->GetWorld()) {
		if (const UUIMessageSubsystem* const uiMessageSubsystem = world->GetSubsystem<UUIMessageSubsystem>()) {
			return uiMessageSubsystem->CheckUIMessage(aUIMessage);
		}
	}

	return false;
}
