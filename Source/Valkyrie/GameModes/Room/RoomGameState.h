// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RoomGameState.generated.h"

class UMapDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelectedMapDataAssetChanged);

UCLASS()
class VALKYRIE_API ARoomGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category="Valkyrie")
	FSelectedMapDataAssetChanged myMapChanged;
	
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	UMapDataAsset* GetMap() const { return myMap; }
	void SetMap(UMapDataAsset* aMapDataAsset);

private:
	UFUNCTION()
	void OnRep_Map();

	UPROPERTY(ReplicatedUsing=OnRep_Map)
	TObjectPtr<UMapDataAsset> myMap{nullptr};
};
