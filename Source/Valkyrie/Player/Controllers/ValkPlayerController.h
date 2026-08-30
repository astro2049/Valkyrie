// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ValkPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UUserWidget;

UCLASS(Blueprintable)
class VALKYRIE_API AValkPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void Client_OnPlayerDied(); // called from player character
	UFUNCTION(Client, Reliable)
	void Client_OnPlayerRespawned(); // called from game mode
	UFUNCTION(Client, Unreliable)
	void Client_PlayHitRepresentations(); // called from weapon component
	UFUNCTION(Client, Unreliable)
	void Client_PlayDamageRepresentations(FVector aDamageSourceLocation); // called from player character
private:
	virtual void BeginPlay() override; // bind input mapping context, add HUD and scoreboard to viewport
	virtual void SetupInputComponent() override; // bind open/close scoreboard action

	// I. input
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputMappingContext> myInputMappingContext{nullptr}; // input mapping context
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myInputActionOpenScoreboard{nullptr}; // toggle scoreboard action

	// II. UI
	// II.1. HUD
	UPROPERTY()
	TObjectPtr<UUserWidget> myHUDWidget{nullptr}; // widget
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myHUDWidgetClass; // class
	// II.2. scoreboard
	void ShowScoreboard();
	void HideScoreboard();

	UPROPERTY()
	TObjectPtr<UUserWidget> myScoreboardWidget{nullptr}; // widget
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myScoreboardWidgetClass; // class
};
