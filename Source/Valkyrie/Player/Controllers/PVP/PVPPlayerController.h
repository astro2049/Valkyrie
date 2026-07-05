// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "PVPPlayerController.generated.h"

class UInputAction;
class UUserWidget;

UCLASS(Abstract)
class VALKYRIE_API APVPPlayerController : public AValkPlayerController
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPlayerDeath() override;
	UFUNCTION()
	void OnPawnChanged(APawn* anOldPawn, APawn* aNewPawn);

	void ShowScoreboard();
	void HideScoreboard();

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
};
