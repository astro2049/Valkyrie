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
	UFUNCTION(BlueprintCallable, Category="Menu|Button")
	void SetLabel(const FText& aLabel);
	UFUNCTION(BlueprintCallable, Category="Menu|Button")
	void SetButtonEnabled(bool aIsEnabled);

	UPROPERTY(BlueprintAssignable, Category="Menu|Button")
	FUIButtonEvent myOnClicked;
	UPROPERTY(BlueprintAssignable, Category="Menu|Button")
	FUIButtonEvent myOnHovered;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void SynchronizeProperties() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Menu|Button")
	TObjectPtr<UButton> myButton{nullptr};
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Menu|Button")
	TObjectPtr<UTextBlock> myText{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Menu|Button|Content")
	FText myLabel;

private:
	UFUNCTION()
	void HandleButtonClicked();
	UFUNCTION()
	void HandleButtonHovered();

	void BindButton();
	void UnbindButton();
};
