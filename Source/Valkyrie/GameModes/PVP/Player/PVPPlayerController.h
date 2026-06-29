// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/ValkGameplayPlayerController.h"
#include "PVPPlayerController.generated.h"

class UInputAction;
class UUIValkHUD;
class UUIPVPHUD;
class UUIPVPScoreboard;

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
	TSubclassOf<UUIValkHUD> myPlayerHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUIPVPHUD> myModeWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUIPVPScoreboard> myScoreboardWidgetClass;

private:
	void ShowScoreboard();
	void HideScoreboard();

	UPROPERTY()
	TObjectPtr<UUIValkHUD> myPlayerHUDWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUIPVPHUD> myModeWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUIPVPScoreboard> myScoreboardWidget{nullptr};
};
