// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ValkPlayerController.generated.h"

class UWidget;
class UInputMappingContext;

UCLASS(Blueprintable)
class VALKYRIE_API AValkPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Valkyrie|Level")
	void RestartCurrentLevel();

protected:
	void AddInputMappingContext() const;
	void SetInputGameOnly(bool aShowMouseCursor = false);
	void SetInputUIOnly(UWidget* aWidgetToFocus, bool aShowMouseCursor = true);

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie|Input")
	TObjectPtr<UInputMappingContext> myInputMappingContext{nullptr};
};
