// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "PVPPlayerController.generated.h"

class UInputAction;
class UUserWidget;
class UHealthComponent;

UCLASS(Abstract)
class VALKYRIE_API APVPPlayerController : public AValkPlayerController
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* anOldPawn, APawn* aNewPawn);
	void SetControlledPawn(const APawn* aPawn);

	void ShowScoreboard();
	void HideScoreboard();
	UFUNCTION()
	void HandlePlayerDied();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myHUDWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> myHUDWidget{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myInputActionOpenScoreboard{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myScoreboardWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> myScoreboardWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UHealthComponent> myHealthComponent{nullptr};
};
