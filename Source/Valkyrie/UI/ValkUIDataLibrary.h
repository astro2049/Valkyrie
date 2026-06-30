// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Valkyrie/GameModes/Extraction/ExtractionTypes.h"
#include "ValkUIDataLibrary.generated.h"

class APlayerController;

USTRUCT(BlueprintType)
struct FValkScoreboardRowData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	FString myPlayerName;
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myKills{0};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myDeaths{0};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myConfirms{0};
};

UCLASS()
class VALKYRIE_API UValkUIDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static void GetPlayerHUDData(
		APlayerController* aPlayerController,
		float& aHealth,
		float& aMaxHealth,
		bool& aShowHealth,
		int32& anAmmo,
		int32& aReserveAmmo,
		bool& aShowAmmo,
		bool& anIsReloading,
		bool& aHasInteractable
	);

	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static void GetExtractionUIData(
		APlayerController* aPlayerController,
		ECombatSliceState& aCombatSliceState,
		float& aDefenseTimeRemaining
	);

	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static void GetTDMUIData(
		APlayerController* aPlayerController,
		int32& aTeamAScore,
		int32& aTeamBScore,
		bool& aMatchEnded,
		int32& aWinningTeamId
	);

	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static void GetKillConfirmedUIData(
		APlayerController* aPlayerController,
		int32& aTeamAScore,
		int32& aTeamBScore,
		bool& aMatchEnded,
		int32& aWinningTeamId
	);

	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static void GetPVPScoreboardData(
		APlayerController* aPlayerController,
		int32& aTeamAScore,
		int32& aTeamBScore,
		bool& aShowConfirms,
		TArray<FValkScoreboardRowData>& someTeamARows,
		TArray<FValkScoreboardRowData>& someTeamBRows
	);
};
