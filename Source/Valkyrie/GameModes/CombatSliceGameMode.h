// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CombatSliceTypes.h"
#include "CombatSliceGameMode.generated.h"

UCLASS()
class VALKYRIE_API ACombatSliceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACombatSliceGameMode();

	UFUNCTION(BlueprintCallable, Category="Combat Slice")
	void StartGenerator();

	UFUNCTION(BlueprintCallable, Category="Combat Slice")
	void CompleteDefense();

	UFUNCTION(BlueprintCallable, Category="Combat Slice")
	void CompleteExtraction();

	UFUNCTION(BlueprintImplementableEvent, Category="Combat Slice")
	void OnDefenseCompleted();

	UFUNCTION(BlueprintImplementableEvent, Category="Combat Slice")
	void OnExtractionCompleted();

	UFUNCTION(BlueprintPure, Category="Combat Slice")
	float GetDefenseTimeRemaining() const { return myDefenseTimeRemaining; }

protected:
	virtual void BeginPlay() override;

private:
	void TickDefenseTimer();

	void SetCombatSliceState(ECombatSliceState aNewState, const FText& anObjectiveText) const;

	void SetDefenseTimer(float aDefenseTimeRemaining, bool aShowDefenseTimer) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Slice", meta=(AllowPrivateAccess="true"))
	float myDefenseDuration{60.f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat Slice", meta=(AllowPrivateAccess="true"))
	float myDefenseTimeRemaining{60.f};

	FTimerHandle myDefenseTimerHandle;
};
