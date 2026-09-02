// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ValkGameMode.generated.h"

class AValkPlayerState;

UCLASS()
class VALKYRIE_API AValkGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AValkGameMode();
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* aNewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* aPlayer) override;
	virtual void PlayerDied(AController* aKillerController, AController* aVictimController);

protected:
	void RespawnPlayer(AController* aController);

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie", meta=(ClampMin="1", ClampMax="2"))
	int32 myTeamCount{1};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myRespawnDelay{3.f};
};
