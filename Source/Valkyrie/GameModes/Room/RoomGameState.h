// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RoomGameState.generated.h"

class UDataAsset_Map;

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
	UDataAsset_Map* GetMap() const { return myMap; }
	void SetMap(UDataAsset_Map* aMapDataAsset);

private:
	UFUNCTION()
	void OnRep_Map();

	UPROPERTY(ReplicatedUsing=OnRep_Map)
	TObjectPtr<UDataAsset_Map> myMap{nullptr};
};
