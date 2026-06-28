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

	virtual AActor* ChoosePlayerStart_Implementation(AController* aPlayer) override;
	virtual void RestartPlayer(AController* aNewPlayer) override;

	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void StartGenerator();
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void CompleteDefense();
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void CompleteExtraction();

	UFUNCTION(BlueprintImplementableEvent, Category="Valkyrie")
	void OnExtractionCompleted();

	UFUNCTION(BlueprintPure, Category="Valkyrie")
	float GetDefenseTimeRemaining() const { return myDefenseTimeRemaining; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandlePlayerDeath();

	void TickDefenseTimer();
	void BindPlayerDeath(AController* aController);
	bool AreAllPlayersDead() const;
	void FailExtraction();
	void SetCombatSliceState(ECombatSliceState aNewState, const FText& anObjectiveText) const;
	void SetDefenseTimer(float aDefenseTimeRemaining, bool aShowDefenseTimer) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myDefenseDuration{60.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myDefenseTimeRemaining{60.f};

	TWeakObjectPtr<AExtractionZone> myExtractionZone;
	FTimerHandle myDefenseTimerHandle;
};
