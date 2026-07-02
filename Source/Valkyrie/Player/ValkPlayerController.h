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
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void ReturnAllPlayersToMainMenu();

protected:
	void AddInputMappingContext() const;
	void SetInputGameOnly();
	void SetInputUIOnly(UWidget* aWidgetToFocus);

private:
	UFUNCTION(Server, Reliable)
	void Server_ReturnAllPlayersToMainMenu();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputMappingContext> myInputMappingContext{nullptr};
};
