// Fill out your copyright notice in the Description page of Project Settings.

#include "UIDataHUDLibrary.h"

#include "GameFramework/PlayerController.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"

FValkHUDData UUIDataHUDLibrary::GetHUDData(APlayerController* const aPlayerController)
{
	FValkHUDData data;
	const AValkPlayerPawn* const playerPawn = aPlayerController
		? Cast<AValkPlayerPawn>(aPlayerController->GetPawn())
		: nullptr;
	if (!playerPawn) {
		return data;
	}

	if (const UHealthComponent* const healthComponent = playerPawn->FindComponentByClass<UHealthComponent>()) {
		data.myHealth = healthComponent->GetHealth();
		data.myMaxHealth = healthComponent->GetMaxHealth();
	}
	if (const UWeaponComponent* const weaponComponent = playerPawn->FindComponentByClass<UWeaponComponent>()) {
		data.myAmmo = weaponComponent->GetAmmoInMag();
		data.myReserveAmmo = weaponComponent->GetReserveAmmo();
		data.myIsReloading = weaponComponent->IsReloading();
	}
	if (const UInteractionComponent* const interactionComponent = playerPawn->FindComponentByClass<UInteractionComponent>()) {
		data.myHasInteractable = interactionComponent->HasInteractable();
	}
	return data;
}

bool UUIDataHUDLibrary::EqualEqual_ValkHUDData(
	const FValkHUDData& aLeft,
	const FValkHUDData& aRight
)
{
	return aLeft == aRight;
}
