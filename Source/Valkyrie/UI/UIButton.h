// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIButton.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUIButtonEvent);

UCLASS(BlueprintType, Blueprintable)
class VALKYRIE_API UUIButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void SetLabel(const FText& aLabel);
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void SetButtonEnabled(bool aIsEnabled);

	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FUIButtonEvent myOnClicked;
	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FUIButtonEvent myOnHovered;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UButton> myButton{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UTextBlock> myText{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie")
	FText myLabel;

private:
	UFUNCTION()
	void HandleButtonClicked();
	UFUNCTION()
	void HandleButtonHovered();

	void BindButton();
	void UnbindButton();
};
