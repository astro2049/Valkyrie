// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/Player/ValkPlayerController.h"
#include "PVPPlayerController.generated.h"

class UHealthComponent;
class UInputAction;
class UPVPHUDViewModel;
class UPVPScoreboardViewModel;
class UUIPVPHUD;
class UUIPVPScoreboard;

UCLASS(Abstract)
class VALKYRIE_API APVPPlayerController : public AValkPlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void AcknowledgePossession(APawn* aPawn) override;

protected:
	virtual void SetupInputComponent() override;
	virtual UPVPHUDViewModel* CreateHUDViewModel() PURE_VIRTUAL(APVPPlayerController::CreateHUDViewModel, return nullptr;);
	virtual UPVPScoreboardViewModel* CreateScoreboardViewModel() PURE_VIRTUAL(APVPPlayerController::CreateScoreboardViewModel, return nullptr;);

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myScoreboardAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUIPVPHUD> myHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UUIPVPScoreboard> myScoreboardWidgetClass;

private:
	void CreatePVPUI();
	void BindUIToSources() const;
	void BindToPawnHealth();
	void ShowScoreboard();
	void HideScoreboard();
	UFUNCTION()
	void HandlePlayerDeath();

	UPROPERTY()
	TObjectPtr<UPVPHUDViewModel> myHUDViewModel{nullptr};
	UPROPERTY()
	TObjectPtr<UPVPScoreboardViewModel> myScoreboardViewModel{nullptr};
	UPROPERTY()
	TObjectPtr<UUIPVPHUD> myHUDWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UUIPVPScoreboard> myScoreboardWidget{nullptr};
	UPROPERTY()
	TObjectPtr<UHealthComponent> myBoundHealthComponent{nullptr};
};
