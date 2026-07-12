// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GunDataAsset.generated.h"

UENUM(BlueprintType)
enum class EValkFireMode : uint8
{
	Auto,
	Single
};

UCLASS()
class VALKYRIE_API UGunDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myDamage{25.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myRPM{600.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	int32 myMagazineSize{30};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	int32 myInitialReserveAmmo{90};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myReloadDuration{2.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myTraceDistance{10000.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	EValkFireMode myFireMode{EValkFireMode::Auto};
};
