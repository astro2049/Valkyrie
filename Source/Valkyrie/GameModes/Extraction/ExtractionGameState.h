// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/ValkGameState.h"
#include "ExtractionTypes.h"
#include "ExtractionGameState.generated.h"

DECLARE_MULTICAST_DELEGATE(FExtractionStateChanged);

UCLASS()
class VALKYRIE_API AExtractionGameState : public AValkGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	FExtractionStateChanged myOnExtractionStateChanged;

private:
	UFUNCTION()
	void OnRep_CombatSliceState();
	UFUNCTION()
	void OnRep_ObjectiveText();
	UFUNCTION()
	void OnRep_DefenseTimer();

	UPROPERTY(ReplicatedUsing=OnRep_CombatSliceState, VisibleAnywhere, BlueprintReadOnly, Category="Combat Slice", meta=(AllowPrivateAccess="true"))
	ECombatSliceState myCombatSliceState{ECombatSliceState::ToStartGenerator};
	UPROPERTY(ReplicatedUsing=OnRep_ObjectiveText, VisibleAnywhere, BlueprintReadOnly, Category="Combat Slice", meta=(AllowPrivateAccess="true"))
	FText myObjectiveText;
	UPROPERTY(ReplicatedUsing=OnRep_DefenseTimer, VisibleAnywhere, BlueprintReadOnly, Category="Combat Slice", meta=(AllowPrivateAccess="true"))
	float myDefenseTimeRemaining{0.f};
	UPROPERTY(ReplicatedUsing=OnRep_DefenseTimer, VisibleAnywhere, BlueprintReadOnly, Category="Combat Slice", meta=(AllowPrivateAccess="true"))
	bool myShowDefenseTimer{false};
};
