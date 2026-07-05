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
	void Client_OnPlayerDeath();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPlayerDeath();

	void AddInputMappingContext() const;
	void SetInputGameOnly();

private:
	UFUNCTION()
	void OnPawnChanged(APawn* anOldPawn, APawn* aNewPawn);
	void ShowScoreboard();
	void HideScoreboard();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputMappingContext> myInputMappingContext{nullptr};
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
