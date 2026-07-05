// Fill out your copyright notice in the Description page of Project Settings.

#include "KillConfirmedTag.h"

#include "Valkyrie/GameModes/PVP//KillConfirmed/KillConfirmedGameMode.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/GameModes/ValkGameState.h"
#include "Valkyrie/Player/States/ValkPlayerState.h"

AKillConfirmedTag::AKillConfirmedTag()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AKillConfirmedTag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKillConfirmedTag, myDroppedTeamId);
}

void AKillConfirmedTag::SetDroppedTeamId(const EValkTeamId aTeamId)
{
	if (HasAuthority()) {
		myDroppedTeamId = aTeamId;
	}
}

void AKillConfirmedTag::HandlePlayerEntered(AActor* const anOtherActor)
{
	if (!HasAuthority()) {
		return;
	}
	UWorld* const world = GetWorld();
	if (!world) {
		return;
	}
	const AValkGameState* const gameState = world->GetGameState<AValkGameState>();
	if (gameState && gameState->HasMatchEnded()) {
		return;
	}

	APawn* const playerPawn = Cast<APawn>(anOtherActor);
	const UHealthComponent* const healthComponent = playerPawn
		? playerPawn->FindComponentByClass<UHealthComponent>()
		: nullptr;
	AValkPlayerState* const playerState = playerPawn && playerPawn->GetController()
		? playerPawn->GetController()->GetPlayerState<AValkPlayerState>()
		: nullptr;
	if (!playerState || (healthComponent && healthComponent->IsDead())) {
		return;
	}

	if (playerState->GetTeamId() == myDroppedTeamId) {
		Destroy();
		return;
	}

	if (AKillConfirmedGameMode* const gameMode = world->GetAuthGameMode<AKillConfirmedGameMode>()) {
		if (gameMode->ConfirmTag(playerPawn)) {
			Destroy();
		}
	}
}
