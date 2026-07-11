// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "ExtractionPlayerController.generated.h"

class UUserWidget;

UCLASS(Blueprintable)
class VALKYRIE_API AExtractionPlayerController : public AValkPlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPlayerDied() override;

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myDeadOverlayWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> myDeadOverlayWidget{nullptr};
};
