// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIDataPVPLibrary.generated.h"

class APlayerController;

USTRUCT(BlueprintType)
struct FValkPVPData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myTeamAScore{0};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myTeamBScore{0};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	bool myMatchEnded{false};
	UPROPERTY(BlueprintReadOnly, Category="Valkyrie")
	int32 myWinningTeamId{INDEX_NONE};

	bool operator==(const FValkPVPData& anOther) const
	{
		return myTeamAScore == anOther.myTeamAScore &&
			myTeamBScore == anOther.myTeamBScore &&
			myMatchEnded == anOther.myMatchEnded &&
			myWinningTeamId == anOther.myWinningTeamId;
	}
};

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
class VALKYRIE_API UUIDataPVPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static FValkPVPData GetTDMData(APlayerController* aPlayerController);
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	static FValkPVPData GetKillConfirmedData(APlayerController* aPlayerController);
	UFUNCTION(BlueprintPure, Category="Valkyrie", meta=(DisplayName="Equal (Valk PVP Mode HUD Data)", CompactNodeTitle="=="))
	static bool EqualEqual_ValkPVPData(
		const FValkPVPData& aLeft,
		const FValkPVPData& aRight
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
