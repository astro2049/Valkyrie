// Fill out your copyright notice in the Description page of Project Settings.

#include "KillConfirmedTag.h"

#include "Valkyrie/GameModes/PVP//KillConfirmed/KillConfirmedGameMode.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/GameModes/ValkGameState.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
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
	if (!anOtherActor) {
		return;
	}
	if (!HasAuthority()) {
		return;
	}

	if (const UWorld* const world = GetWorld()) {
		if (const APawn* const playerPawn = Cast<APawn>(anOtherActor)) {
			if (const AController* const playerController = playerPawn->GetController()) {
				if (const AValkPlayerState* const playerState = playerController->GetPlayerState<AValkPlayerState>()) {
					if (playerState->GetTeamId() == myDroppedTeamId) {
						Destroy();
					} else if (AKillConfirmedGameMode* const gameMode = world->GetAuthGameMode<AKillConfirmedGameMode>()) {
						if (gameMode->ConfirmTag(playerPawn)) {
							Destroy();
						}
					}
				}
			}
		}
	}
}
