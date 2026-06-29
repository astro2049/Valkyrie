// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/ValkGameplayPlayerController.h"
#include "ExtractionPlayerController.generated.h"

class UUIExtractionHUD;
class UUIValkHUD;
class UUserWidget;

UCLASS(Blueprintable)
class VALKYRIE_API AExtractionPlayerController : public AValkGameplayPlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float aDeltaSeconds) override;

protected:
	virtual void HandleLocalPlayerDeath() override;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUIValkHUD> myPlayerHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUIExtractionHUD> myModeWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myDeathMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myDeadOverlayWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myVictoryMenuWidgetClass;

private:
	void UpdateEndMenuFromState();
	void ShowDeadOverlay();
	void ShowFailureMenu();
	void ShowVictoryMenu();

	UPROPERTY()
	TObjectPtr<UUIValkHUD> myPlayerHUDWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUIExtractionHUD> myModeWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myDeathMenuWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myDeadOverlayWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myVictoryMenuWidget{nullptr};
};
