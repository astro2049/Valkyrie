// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_HUD.generated.h"

/**
 * 
 */
UCLASS()
class VALKYRIE_API UUI_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Valkyrie")
	void ShowDamageIndicator(FVector aDamageSourceLocation);
	void ToggleInputActionsMenu();

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Valkyrie")
	void ShowInputActionsMenu();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Valkyrie")
	void HideInputActionsMenu();

private:
	bool myIsShowingInputActionsMenu{false};
};
