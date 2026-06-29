// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkHUDViewModel.h"

#include "GameFramework/PlayerController.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"

void UValkHUDViewModel::RefreshData()
{
	const AValkPlayerPawn* const playerPawn = GetPlayerPawn();
	RefreshWeaponHUDData(playerPawn);
	RefreshHealthHUDData(playerPawn);
	RefreshInteractionHUDData(playerPawn);
}

const AValkPlayerPawn* UValkHUDViewModel::GetPlayerPawn() const
{
	const APlayerController* const playerController = GetPlayerController();
	return playerController ? Cast<AValkPlayerPawn>(playerController->GetPawn()) : nullptr;
}

void UValkHUDViewModel::RefreshWeaponHUDData(const AValkPlayerPawn* const aPlayerPawn)
{
	myWeaponHUDData = {};
	if (aPlayerPawn) {
		if (const UWeaponComponent* const weaponComponent = aPlayerPawn->FindComponentByClass<UWeaponComponent>()) {
			myWeaponHUDData.myAmmoInMag = weaponComponent->GetAmmoInMag();
			myWeaponHUDData.myReserveAmmo = weaponComponent->GetReserveAmmo();
			myWeaponHUDData.myIsReloading = weaponComponent->IsReloading();
			myWeaponHUDData.myShowAmmo = true;
		}
	}
}

void UValkHUDViewModel::RefreshHealthHUDData(const AValkPlayerPawn* const aPlayerPawn)
{
	myHealthHUDData = {};
	if (aPlayerPawn) {
		if (const UHealthComponent* const healthComponent = aPlayerPawn->FindComponentByClass<UHealthComponent>()) {
			myHealthHUDData.myHealth = healthComponent->GetHealth();
			myHealthHUDData.myMaxHealth = healthComponent->GetMaxHealth();
			myHealthHUDData.myShowHealth = true;
			myHealthHUDData.myIsDead = healthComponent->IsDead();
		}
	}
}

void UValkHUDViewModel::RefreshInteractionHUDData(const AValkPlayerPawn* const aPlayerPawn)
{
	myShowInteractPrompt = false;
	if (aPlayerPawn) {
		if (const UInteractionComponent* const interactionComponent = aPlayerPawn->FindComponentByClass<UInteractionComponent>()) {
			myShowInteractPrompt = interactionComponent && interactionComponent->HasInteractable();
		}
	}
}
