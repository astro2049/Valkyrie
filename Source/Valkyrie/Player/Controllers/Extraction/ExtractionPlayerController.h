// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/Controllers/ValkGameplayPlayerController.h"
#include "ExtractionPlayerController.generated.h"

class UUserWidget;

UCLASS(Blueprintable)
class VALKYRIE_API AExtractionPlayerController : public AValkGameplayPlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	virtual void HandleLocalPlayerDeath() override;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myDeadOverlayWidgetClass;

private:
	void ShowDeadOverlay();

	UPROPERTY()
	TObjectPtr<UUserWidget> myHUDWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myDeadOverlayWidget{nullptr};
};
