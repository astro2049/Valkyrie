// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/ValkGameplayPlayerController.h"
#include "PVPPlayerController.generated.h"

class UInputAction;
class UUserWidget;

UCLASS(Abstract)
class VALKYRIE_API APVPPlayerController : public AValkGameplayPlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void HandleLocalPlayerDeath() override;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myScoreboardAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myPlayerHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myModeWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myScoreboardWidgetClass;

private:
	void ShowScoreboard();
	void HideScoreboard();

	UPROPERTY()
	TObjectPtr<UUserWidget> myPlayerHUDWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myModeWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myScoreboardWidget{nullptr};
};
