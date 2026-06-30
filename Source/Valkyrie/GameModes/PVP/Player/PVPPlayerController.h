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
	TSubclassOf<UUserWidget> myHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myScoreboardWidgetClass;

private:
	void ShowScoreboard();
	void HideScoreboard();

	UPROPERTY()
	TObjectPtr<UUserWidget> myHUDWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myScoreboardWidget{nullptr};
};
