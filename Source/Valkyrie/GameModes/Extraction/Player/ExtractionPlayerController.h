// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/ValkGameplayPlayerController.h"
#include "ExtractionPlayerController.generated.h"

class UUIExtractionHUD;
class UExtractionHUDViewModel;
class UUserWidget;
class AExtractionGameState;

UCLASS(Blueprintable)
class VALKYRIE_API AExtractionPlayerController : public AValkGameplayPlayerController
{
	GENERATED_BODY()

protected:
	virtual void CreateModeUI() override;
	virtual void InitializeModeState() override;
	virtual void SetModePawn(AValkPlayerPawn* aPlayerPawn) override;
	virtual void HandleLocalPlayerDeath() override;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUIExtractionHUD> myHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myDeathMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myDeadOverlayWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myVictoryMenuWidgetClass;

private:
	void ShowDeadOverlay();
	void ShowFailureMenu();
	void ShowVictoryMenu();

	void HandleExtractionStateChanged();

	UPROPERTY()
	TObjectPtr<UExtractionHUDViewModel> myHUDViewModel{nullptr};
	UPROPERTY()
	TObjectPtr<UUIExtractionHUD> myHUDWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myDeathMenuWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myDeadOverlayWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myVictoryMenuWidget{nullptr};
	TWeakObjectPtr<AExtractionGameState> myBoundExtractionGameState;
};
