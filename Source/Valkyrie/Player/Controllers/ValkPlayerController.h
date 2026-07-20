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
	void OnDied(AController* aKillerController = nullptr);

	UFUNCTION(Client, Reliable)
	void Client_OnPlayerDied();
	UFUNCTION(Client, Reliable)
	void Client_OnPlayerRespawned();
	UFUNCTION(Client, Unreliable)
	void Client_PlayHitRepresentations();
	UFUNCTION(Client, Unreliable)
	void Client_PlayDamageRepresentations(FVector aDamageSourceLocation);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPlayerDied();
	virtual void OnPlayerRespawned();

private:
	void ShowScoreboard();
	void HideScoreboard();

	// I. input
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputMappingContext> myInputMappingContext{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myInputActionOpenScoreboard{nullptr};

	// II. UI
	// HUD
	UPROPERTY()
	TObjectPtr<UUserWidget> myHUDWidget{nullptr};
	// scoreboard
	UPROPERTY()
	TObjectPtr<UUserWidget> myScoreboardWidget{nullptr};
	// HUD class
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myHUDWidgetClass;
	// scoreboard class
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUserWidget> myScoreboardWidgetClass;
};
