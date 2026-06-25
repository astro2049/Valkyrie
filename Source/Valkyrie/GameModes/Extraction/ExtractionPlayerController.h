// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/ValkPlayerController.h"
#include "ExtractionTypes.h"
#include "ExtractionPlayerController.generated.h"

class UUIExtractionHUD;
class UHealthComponent;
class UInteractionComponent;
class UExtractionHUDViewModel;
class UUserWidget;
class AExtractionGameState;

UCLASS(Blueprintable)
class VALKYRIE_API AExtractionPlayerController : public AValkPlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void AcknowledgePossession(APawn* aPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Combat Slice|UI")
	TSubclassOf<UUIExtractionHUD> myHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Combat Slice|UI")
	TSubclassOf<UUserWidget> myDeathMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Combat Slice|UI")
	TSubclassOf<UUserWidget> myDeadOverlayWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Combat Slice|UI")
	TSubclassOf<UUserWidget> myVictoryMenuWidgetClass;

private:
	void CreateHUDAndViewModel();
	void BindHUDToPawn();
	void BindHUDToGameState();
	void ShowDeadOverlay();
	void ShowFailureMenu();
	void ShowVictoryMenu();

	UFUNCTION()
	void HandlePlayerDeath();
	UFUNCTION()
	void HandleCombatSliceStateChanged(ECombatSliceState aCombatSliceState);
	UFUNCTION()
	void HandleInteractableChanged(bool aHasInteractable);

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
	UPROPERTY()
	TObjectPtr<UHealthComponent> myBoundHealthComponent{nullptr};
	UPROPERTY()
	TObjectPtr<AExtractionGameState> myBoundCombatSliceGameState{nullptr};
};
