// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UIMainMenu.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CreateMenuScreen();
	ConfigureMenuInput();
}

void AMainMenuPlayerController::HandleQuitRequested()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}

void AMainMenuPlayerController::HandleStartRequested(UMapDataAsset* aMapData, FMenuModeEntry aModeEntry)
{
	OpenSelectedMapMode(aMapData, aModeEntry);
}

void AMainMenuPlayerController::CreateMenuScreen()
{
	if (myMenuScreenWidgetClass) {
		myMenuScreenWidget = CreateWidget<UUIMainMenu>(this, myMenuScreenWidgetClass);
		if (myMenuScreenWidget) {
			myMenuScreenWidget->myOnQuitRequested.AddUniqueDynamic(this, &AMainMenuPlayerController::HandleQuitRequested);
			myMenuScreenWidget->myOnStartRequested.AddUniqueDynamic(this, &AMainMenuPlayerController::HandleStartRequested);
			myMenuScreenWidget->AddToViewport(myMenuZOrder);
		}
	}
}

void AMainMenuPlayerController::ConfigureMenuInput()
{
	bShowMouseCursor = true;

	FInputModeUIOnly inputMode;
	if (myMenuScreenWidget) {
		inputMode.SetWidgetToFocus(myMenuScreenWidget->TakeWidget());
	}

	SetInputMode(inputMode);
}

void AMainMenuPlayerController::OpenSelectedMapMode(UMapDataAsset* aMapData, const FMenuModeEntry& aModeEntry)
{
	if (!aMapData) {
		return;
	}

	if (!aMapData->myLevelName.IsNone() && aModeEntry.IsPlayable()) {
		const FString gameModePath = aModeEntry.myGameModeClass->GetPathName();
		const FString options = FString::Printf(TEXT("game=%s"), *gameModePath);

		UGameplayStatics::OpenLevel(this, aMapData->myLevelName, true, options);
	}
}
