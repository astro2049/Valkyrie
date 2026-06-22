// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Valkyrie/GameModes/ValkGameMode.h"
#include "ExtractionTypes.h"
#include "ExtractionGameMode.generated.h"

class AExtractionZone;

UCLASS()
class VALKYRIE_API AExtractionGameMode : public AValkGameMode
{
	GENERATED_BODY()

public:
	AExtractionGameMode();

	UFUNCTION(BlueprintCallable, Category="Combat Slice")
	void StartGenerator();
	UFUNCTION(BlueprintCallable, Category="Combat Slice")
	void CompleteDefense();
	UFUNCTION(BlueprintCallable, Category="Combat Slice")
	void CompleteExtraction();

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

	TWeakObjectPtr<AExtractionZone> myExtractionZone;
	FTimerHandle myDefenseTimerHandle;
};
