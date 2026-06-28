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
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void InitializeMapEntry(int32 anEntryIndex, const FText& aDisplayName);
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void SetSelected(bool aIsSelected);

	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FMenuMapEntryEvent myOnHovered;
	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FMenuMapEntryEvent myOnClicked;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="Valkyrie")
	TObjectPtr<UUIButton> myButton{nullptr};

	UFUNCTION(BlueprintImplementableEvent, Category="Valkyrie")
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
