// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MapListPanel//MapDataAsset.h"
#include "MainMenuPlayerController.generated.h"

class UUIMainMenu;

UCLASS(BlueprintType, Blueprintable)
class VALKYRIE_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Menu")
	TSubclassOf<UUIMainMenu> myMenuScreenWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Menu")
	int32 myMenuZOrder{0};

private:
	UFUNCTION()
	void HandleQuitRequested();
	UFUNCTION()
	void HandleStartRequested(UMapDataAsset* aMapData, FMenuModeEntry aModeEntry);

	void CreateMenuScreen();
	void ConfigureMenuInput();
	void OpenSelectedMapMode(UMapDataAsset* aMapData, const FMenuModeEntry& aModeEntry);

	UPROPERTY()
	TObjectPtr<UUIMainMenu> myMenuScreenWidget{nullptr};
};
