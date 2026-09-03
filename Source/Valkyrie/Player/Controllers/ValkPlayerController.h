// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ValkPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(Blueprintable)
class VALKYRIE_API AValkPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void Client_OnPlayerDied(); // called from player character
	UFUNCTION(Client, Reliable)
	void Client_OnPlayerRespawned(); // called from game mode
	UFUNCTION(Client, Reliable)
	void Client_PlayHitRepresentations(); // called from weapon component
	UFUNCTION(Client, Reliable)
	void Client_PlayDamageRepresentations(FVector aDamageSourceLocation); // called from player character
private:
	virtual void BeginPlay() override; // bind input mapping context, add HUD and scoreboard to viewport
	virtual void SetupInputComponent() override; // bind open/close scoreboard action

	// I. input
	void SetInputModeUIOnly();
	void SetInputModeGameOnly();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputMappingContext> myGameplayInputMappingContext{nullptr}; // gameplay input mapping context
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myInputActionOpenScoreboard{nullptr}; // toggle scoreboard action
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myInputActionEscMenuOpen{nullptr}; // open scoreboard action
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputMappingContext> myUIInputMappingContext{nullptr}; // UI input mapping context
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myInputActionEscMenuClose{nullptr}; // close scoreboard action

	// II. Widget Functions
	void ShowScoreboard() { myScoreboardWidget->SetVisibility(ESlateVisibility::Visible); }
	void HideScoreboard() { myScoreboardWidget->SetVisibility(ESlateVisibility::Hidden); }
	void OpenEscMenu();
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void CloseEscMenu();
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void ReturnToMainMenu();

	// III. Widget Members
	UPROPERTY()
	TObjectPtr<UUserWidget> myHUDWidget{nullptr}; // widget
	UPROPERTY()
	TObjectPtr<UUserWidget> myScoreboardWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUserWidget> myEscMenuWidget{nullptr};

	// IV. Widget Classes
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myHUDWidgetClass; // class
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myScoreboardWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myEscMenuWidgetClass;
};
