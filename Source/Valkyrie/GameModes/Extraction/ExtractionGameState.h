// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/ValkGameState.h"
#include "ExtractionTypes.h"
#include "ExtractionGameState.generated.h"

UCLASS()
class VALKYRIE_API AExtractionGameState : public AValkGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetCombatSliceState(ECombatSliceState aCombatSliceState);
	void SetDefenseTimeRemaining(float aDefenseTimeRemaining);

	UFUNCTION(BlueprintPure, Category="Valkyrie")
	ECombatSliceState GetCombatSliceState() const { return myCombatSliceState; }
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	float GetDefenseTimeRemaining() const { return myDefenseTimeRemaining; }

private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	ECombatSliceState myCombatSliceState{ECombatSliceState::ToStartGenerator};
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myDefenseTimeRemaining{0.f};
};
