// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ValkPlayerController.h"
#include "ValkGameplayPlayerController.generated.h"

class AValkPlayerPawn;
class UHealthComponent;

UCLASS(Abstract)
class VALKYRIE_API AValkGameplayPlayerController : public AValkPlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	virtual void CreateModeUI() PURE_VIRTUAL(AValkGameplayPlayerController::CreateModeUI, );
	virtual void InitializeModeState() PURE_VIRTUAL(AValkGameplayPlayerController::InitializeModeState, );
	virtual void SetModePawn(AValkPlayerPawn* aPlayerPawn) PURE_VIRTUAL(AValkGameplayPlayerController::SetModePawn, );
	virtual void HandleLocalPlayerDeath() PURE_VIRTUAL(AValkGameplayPlayerController::HandleLocalPlayerDeath, );

private:
	void SetControlledPawn(APawn* aPawn);
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* anOldPawn, APawn* aNewPawn);
	UFUNCTION()
	void HandleHealthDeath();

	UPROPERTY()
	TObjectPtr<UHealthComponent> myBoundHealthComponent{nullptr};
};
