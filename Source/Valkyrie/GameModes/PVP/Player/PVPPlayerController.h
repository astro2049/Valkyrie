// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/ValkGameplayPlayerController.h"
#include "PVPPlayerController.generated.h"

class UInputAction;
class UPVPHUDViewModel;
class UPVPScoreboardViewModel;
class UUIPVPHUD;
class UUIPVPScoreboard;

UCLASS(Abstract)
class VALKYRIE_API APVPPlayerController : public AValkGameplayPlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;
	virtual void CreateModeUI() override;
	virtual void InitializeModeState() override;
	virtual void SetModePawn(AValkPlayerPawn* aPlayerPawn) override;
	virtual void HandleLocalPlayerDeath() override;
	virtual UPVPHUDViewModel* CreateHUDViewModel() PURE_VIRTUAL(APVPPlayerController::CreateHUDViewModel, return nullptr;);
	virtual UPVPScoreboardViewModel* CreateScoreboardViewModel() PURE_VIRTUAL(APVPPlayerController::CreateScoreboardViewModel, return nullptr;);

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myScoreboardAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUIPVPHUD> myHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUIPVPScoreboard> myScoreboardWidgetClass;

private:
	void ShowScoreboard();
	void HideScoreboard();

	UPROPERTY()
	TObjectPtr<UPVPHUDViewModel> myHUDViewModel{nullptr};
	UPROPERTY()
	TObjectPtr<UPVPScoreboardViewModel> myScoreboardViewModel{nullptr};
	UPROPERTY()
	TObjectPtr<UUIPVPHUD> myHUDWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUIPVPScoreboard> myScoreboardWidget{nullptr};
};
