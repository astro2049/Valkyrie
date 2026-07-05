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

	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void StartGenerator();
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void CompleteDefense();
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void CompleteExtraction();

	UFUNCTION(BlueprintPure, Category="Valkyrie")
	float GetDefenseTimeRemaining() const { return myDefenseTimeRemaining; }

protected:
	virtual void BeginPlay() override;
	virtual void OnPlayerDeath(AController* aKillerController, AController* aVictimController) override;

private:
	void TickDefenseTimer();
	bool AreAllPlayersDead() const;
	void FailExtraction();
	void SetCombatSliceState(ECombatSliceState aNewState) const;
	void SetDefenseTimeRemaining(float aDefenseTimeRemaining) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myDefenseDuration{60.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myDefenseTimeRemaining{60.f};

	TWeakObjectPtr<AExtractionZone> myExtractionZone;
	FTimerHandle myDefenseTimerHandle;
};
