// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkViewModel.h"

#include "GameFramework/PlayerController.h"

void UValkViewModel::Initialize(APlayerController* const aPlayerController)
{
	myPlayerController = aPlayerController;
}
