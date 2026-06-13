// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIGameModeItem.generated.h"

class UUIButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMenuModeEntryEvent, int32, anEntryIndex);

UCLASS(BlueprintType, Blueprintable)
class VALKYRIE_API UUIGameModeItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Menu|Mode Entry")
	void InitializeModeEntry(int32 anEntryIndex, const FText& aDisplayName, bool aIsPlayable);
	UFUNCTION(BlueprintCallable, Category="Menu|Mode Entry")
	void SetSelected(bool aIsSelected);

	UPROPERTY(BlueprintAssignable, Category="Menu|Mode Entry")
	FMenuModeEntryEvent myOnClicked;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Menu|Mode Entry")
	TObjectPtr<UUIButton> myButton{nullptr};

	UFUNCTION(BlueprintImplementableEvent, Category="Menu|Mode Entry")
	void HandleSelectedChanged(bool aIsSelected);

private:
	UFUNCTION()
	void HandleButtonClicked();

	void BindButton();
	void UnbindButton();

	int32 myEntryIndex{INDEX_NONE};
	bool myIsPlayable{false};
};
