// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkUIDataLibrary.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/GameModes/Extraction/ExtractionGameState.h"
#include "Valkyrie/GameModes/KillConfirmed/KillConfirmedGameState.h"
#include "Valkyrie/GameModes/KillConfirmed/Player/KillConfirmedPlayerState.h"
#include "Valkyrie/GameModes/PVP/PVPGameState.h"
#include "Valkyrie/GameModes/PVP/Player/PVPPlayerState.h"
#include "Valkyrie/GameModes/PVP/PVPTypes.h"
#include "Valkyrie/GameModes/TDM/TDMGameState.h"
#include "Valkyrie/GameModes/TDM/Player/TDMPlayerState.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"

void UValkUIDataLibrary::GetPlayerHUDData(
	APlayerController* const aPlayerController,
	float& aHealth,
	float& aMaxHealth,
	bool& aShowHealth,
	int32& anAmmo,
	int32& aReserveAmmo,
	bool& aShowAmmo,
	bool& anIsReloading,
	bool& aHasInteractable
)
{
	aHealth = 0.f;
	aMaxHealth = 0.f;
	aShowHealth = false;
	anAmmo = 0;
	aReserveAmmo = 0;
	aShowAmmo = false;
	anIsReloading = false;
	aHasInteractable = false;

	const AValkPlayerPawn* const playerPawn = aPlayerController
		? Cast<AValkPlayerPawn>(aPlayerController->GetPawn())
		: nullptr;
	if (!playerPawn) {
		return;
	}

	if (const UHealthComponent* const healthComponent = playerPawn->FindComponentByClass<UHealthComponent>()) {
		aHealth = healthComponent->GetHealth();
		aMaxHealth = healthComponent->GetMaxHealth();
		aShowHealth = true;
	}
	if (const UWeaponComponent* const weaponComponent = playerPawn->FindComponentByClass<UWeaponComponent>()) {
		anAmmo = weaponComponent->GetAmmoInMag();
		aReserveAmmo = weaponComponent->GetReserveAmmo();
		aShowAmmo = true;
		anIsReloading = weaponComponent->IsReloading();
	}
	if (const UInteractionComponent* const interactionComponent = playerPawn->FindComponentByClass<UInteractionComponent>()) {
		aHasInteractable = interactionComponent->HasInteractable();
	}
}

void UValkUIDataLibrary::GetExtractionUIData(
	APlayerController* const aPlayerController,
	ECombatSliceState& aCombatSliceState,
	float& aDefenseTimeRemaining
)
{
	aCombatSliceState = ECombatSliceState::ToStartGenerator;
	aDefenseTimeRemaining = 0.f;

	const UWorld* const world = aPlayerController ? aPlayerController->GetWorld() : nullptr;
	if (const AExtractionGameState* const gameState = world ? world->GetGameState<AExtractionGameState>() : nullptr) {
		aCombatSliceState = gameState->GetCombatSliceState();
		aDefenseTimeRemaining = gameState->GetDefenseTimeRemaining();
	}
}

void UValkUIDataLibrary::GetTDMUIData(
	APlayerController* const aPlayerController,
	int32& aTeamAScore,
	int32& aTeamBScore,
	bool& aMatchEnded,
	int32& aWinningTeamId
)
{
	aTeamAScore = 0;
	aTeamBScore = 0;
	aMatchEnded = false;
	aWinningTeamId = ValkTeamId::None;

	const UWorld* const world = aPlayerController ? aPlayerController->GetWorld() : nullptr;
	if (const ATDMGameState* const gameState = world ? world->GetGameState<ATDMGameState>() : nullptr) {
		aTeamAScore = gameState->GetTeamAKills();
		aTeamBScore = gameState->GetTeamBKills();
		aMatchEnded = gameState->HasMatchEnded();
		aWinningTeamId = gameState->GetWinningTeamId();
	}
}

void UValkUIDataLibrary::GetKillConfirmedUIData(
	APlayerController* const aPlayerController,
	int32& aTeamAScore,
	int32& aTeamBScore,
	bool& aMatchEnded,
	int32& aWinningTeamId
)
{
	aTeamAScore = 0;
	aTeamBScore = 0;
	aMatchEnded = false;
	aWinningTeamId = ValkTeamId::None;

	const UWorld* const world = aPlayerController ? aPlayerController->GetWorld() : nullptr;
	if (const AKillConfirmedGameState* const gameState = world ? world->GetGameState<AKillConfirmedGameState>() : nullptr) {
		aTeamAScore = gameState->GetTeamAConfirms();
		aTeamBScore = gameState->GetTeamBConfirms();
		aMatchEnded = gameState->HasMatchEnded();
		aWinningTeamId = gameState->GetWinningTeamId();
	}
}

void UValkUIDataLibrary::GetPVPScoreboardData(
	APlayerController* const aPlayerController,
	int32& aTeamAScore,
	int32& aTeamBScore,
	bool& aShowConfirms,
	TArray<FValkScoreboardRowData>& someTeamARows,
	TArray<FValkScoreboardRowData>& someTeamBRows
)
{
	aTeamAScore = 0;
	aTeamBScore = 0;
	aShowConfirms = false;
	someTeamARows.Reset();
	someTeamBRows.Reset();

	const UWorld* const world = aPlayerController ? aPlayerController->GetWorld() : nullptr;
	const APVPGameState* const gameState = world ? world->GetGameState<APVPGameState>() : nullptr;
	if (!gameState) {
		return;
	}

	if (const AKillConfirmedGameState* const killConfirmedGameState = Cast<AKillConfirmedGameState>(gameState)) {
		aTeamAScore = killConfirmedGameState->GetTeamAConfirms();
		aTeamBScore = killConfirmedGameState->GetTeamBConfirms();
		aShowConfirms = true;
	} else if (const ATDMGameState* const tdmGameState = Cast<ATDMGameState>(gameState)) {
		aTeamAScore = tdmGameState->GetTeamAKills();
		aTeamBScore = tdmGameState->GetTeamBKills();
	}

	for (const APlayerState* const playerState : gameState->PlayerArray) {
		const APVPPlayerState* const pvpPlayerState = Cast<APVPPlayerState>(playerState);
		if (!pvpPlayerState) {
			continue;
		}

		FValkScoreboardRowData rowData;
		rowData.myPlayerName = pvpPlayerState->GetPlayerName();
		if (const AKillConfirmedPlayerState* const killConfirmedPlayerState = Cast<AKillConfirmedPlayerState>(playerState)) {
			rowData.myKills = killConfirmedPlayerState->GetKills();
			rowData.myDeaths = killConfirmedPlayerState->GetDeaths();
			rowData.myConfirms = killConfirmedPlayerState->GetConfirms();
		} else if (const ATDMPlayerState* const tdmPlayerState = Cast<ATDMPlayerState>(playerState)) {
			rowData.myKills = tdmPlayerState->GetKills();
			rowData.myDeaths = tdmPlayerState->GetDeaths();
		} else {
			continue;
		}

		if (pvpPlayerState->GetTeamId() == ValkTeamId::TeamA) {
			someTeamARows.Add(MoveTemp(rowData));
		} else if (pvpPlayerState->GetTeamId() == ValkTeamId::TeamB) {
			someTeamBRows.Add(MoveTemp(rowData));
		}
	}

	auto sortRows = [](const FValkScoreboardRowData& aLeft, const FValkScoreboardRowData& aRight) {
		if (aLeft.myKills != aRight.myKills) {
			return aLeft.myKills > aRight.myKills;
		}
		if (aLeft.myDeaths != aRight.myDeaths) {
			return aLeft.myDeaths < aRight.myDeaths;
		}
		return aLeft.myPlayerName < aRight.myPlayerName;
	};
	someTeamARows.Sort(sortRows);
	someTeamBRows.Sort(sortRows);
}
