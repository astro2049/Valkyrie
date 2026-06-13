// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIMainMenuPanel.generated.h"

class UUIButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMainMenuEvent);

UCLASS(BlueprintType, Blueprintable)
class VALKYRIE_API UUIMainMenuPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Menu|Main")
	FMainMenuEvent myOnSelectMapRequested;
	UPROPERTY(BlueprintAssignable, Category="Menu|Main")
	FMainMenuEvent myOnQuitRequested;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Menu|Main")
	TObjectPtr<UUIButton> mySelectMapButton{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Menu|Main")
	TObjectPtr<UUIButton> myQuitButton{nullptr};

private:
	UFUNCTION()
	void HandleSelectMapClicked();
	UFUNCTION()
	void HandleQuitClicked();

	void BindButtons();
	void UnbindButtons();
};
