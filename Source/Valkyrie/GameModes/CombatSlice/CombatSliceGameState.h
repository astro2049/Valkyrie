// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CombatSliceTypes.h"
#include "CombatSliceGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FCombatSliceStateChanged,
	ECombatSliceState, aCombatSliceState
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FObjectiveChanged,
	FText, anObjectiveText
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FDefenseTimerChanged,
	float, aDefenseTimeRemaining,
	bool, aShowDefenseTimer
);

UCLASS()
class VALKYRIE_API ACombatSliceGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void SetCombatSliceState(ECombatSliceState aCombatSliceState);
	void SetObjectiveText(const FText& anObjectiveText);
	void SetDefenseTimer(float aDefenseTimeRemaining, bool aShowDefenseTimer);

	UFUNCTION(BlueprintPure, Category="Combat Slice")
	ECombatSliceState GetCombatSliceState() const { return myCombatSliceState; }
	UFUNCTION(BlueprintPure, Category="Combat Slice")
	FText GetObjectiveText() const { return myObjectiveText; }
	UFUNCTION(BlueprintPure, Category="Combat Slice")
	float GetDefenseTimeRemaining() const { return myDefenseTimeRemaining; }
	UFUNCTION(BlueprintPure, Category="Combat Slice")
	bool ShouldShowDefenseTimer() const { return myShowDefenseTimer; }

	UPROPERTY(BlueprintAssignable, Category="Combat Slice")
	FCombatSliceStateChanged OnCombatSliceStateChanged;
	UPROPERTY(BlueprintAssignable, Category="Combat Slice")
	FObjectiveChanged OnObjectiveChanged;
	UPROPERTY(BlueprintAssignable, Category="Combat Slice")
	FDefenseTimerChanged OnDefenseTimerChanged;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Slice", meta=(AllowPrivateAccess="true"))
	ECombatSliceState myCombatSliceState{ECombatSliceState::ToStartGenerator};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Slice", meta=(AllowPrivateAccess="true"))
	FText myObjectiveText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Slice", meta=(AllowPrivateAccess="true"))
	float myDefenseTimeRemaining{0.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Slice", meta=(AllowPrivateAccess="true"))
	bool myShowDefenseTimer{false};
};
