// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIMapItem.generated.h"

class UUIButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMenuMapEntryEvent, int32, anEntryIndex);

UCLASS(BlueprintType, Blueprintable)
class VALKYRIE_API UUIMapItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Menu|Map Entry")
	void InitializeMapEntry(int32 anEntryIndex, const FText& aDisplayName);
	UFUNCTION(BlueprintCallable, Category="Menu|Map Entry")
	void SetSelected(bool aIsSelected);

	UPROPERTY(BlueprintAssignable, Category="Menu|Map Entry")
	FMenuMapEntryEvent myOnHovered;
	UPROPERTY(BlueprintAssignable, Category="Menu|Map Entry")
	FMenuMapEntryEvent myOnClicked;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Menu|Map Entry")
	TObjectPtr<UUIButton> myButton{nullptr};

	UFUNCTION(BlueprintImplementableEvent, Category="Menu|Map Entry")
	void HandleSelectedChanged(bool aIsSelected);

private:
	UFUNCTION()
	void HandleButtonHovered();
	UFUNCTION()
	void HandleButtonClicked();

	void BindButton();
	void UnbindButton();

	int32 myEntryIndex{INDEX_NONE};
};
