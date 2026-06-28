// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkHUDViewModel.h"

#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"

void UValkHUDViewModel::BindToPawn(AValkPlayerPawn* const aPlayerPawn)
{
	if (myPlayerPawn.Get() == aPlayerPawn) {
		RefreshPawnData();
		return;
	}

	myPlayerPawn = aPlayerPawn;
	if (const AValkPlayerPawn* const playerPawn = myPlayerPawn.Get()) {
		if (UWeaponComponent* const weaponComponent = playerPawn->FindComponentByClass<UWeaponComponent>()) {
			weaponComponent->myOnWeaponStateChanged.AddUObject(
				this,
				&UValkHUDViewModel::OnWeaponStateChanged
			);
		}
		if (UHealthComponent* const healthComponent = playerPawn->FindComponentByClass<UHealthComponent>()) {
			healthComponent->myOnHealthStateChanged.AddUObject(
				this,
				&UValkHUDViewModel::OnHealthStateChanged
			);
		}
		if (UInteractionComponent* const interactionComponent = playerPawn->FindComponentByClass<UInteractionComponent>()) {
			interactionComponent->myOnInteractionStateChanged.AddUObject(
				this,
				&UValkHUDViewModel::OnInteractionStateChanged
			);
		}
	}
	RefreshPawnData();
}

void UValkHUDViewModel::RefreshPawnData()
{
	RefreshWeaponHUDData();
	RefreshHealthHUDData();
	RefreshInteractionHUDData();
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::RefreshWeaponHUDData()
{
	myWeaponHUDData = {};
	if (const AValkPlayerPawn* const playerPawn = myPlayerPawn.Get()) {
		if (const UWeaponComponent* const weaponComponent = playerPawn->FindComponentByClass<UWeaponComponent>()) {
			myWeaponHUDData.myAmmoInMag = weaponComponent->GetAmmoInMag();
			myWeaponHUDData.myReserveAmmo = weaponComponent->GetReserveAmmo();
			myWeaponHUDData.myIsReloading = weaponComponent->IsReloading();
			myWeaponHUDData.myShowAmmo = true;
		}
	}
}

void UValkHUDViewModel::RefreshHealthHUDData()
{
	myHealthHUDData = {};
	if (const AValkPlayerPawn* const playerPawn = myPlayerPawn.Get()) {
		if (const UHealthComponent* const healthComponent = playerPawn->FindComponentByClass<UHealthComponent>()) {
			myHealthHUDData.myHealth = healthComponent->GetHealth();
			myHealthHUDData.myMaxHealth = healthComponent->GetMaxHealth();
			myHealthHUDData.myShowHealth = true;
			myHealthHUDData.myIsDead = healthComponent->IsDead();
		}
	}
}

void UValkHUDViewModel::RefreshInteractionHUDData()
{
	myShowInteractPrompt = false;
	if (const AValkPlayerPawn* const playerPawn = myPlayerPawn.Get()) {
		if (const UInteractionComponent* const interactionComponent = playerPawn->FindComponentByClass<UInteractionComponent>()) {
			myShowInteractPrompt = interactionComponent && interactionComponent->HasInteractable();
		}
	}
}

void UValkHUDViewModel::OnWeaponStateChanged()
{
	RefreshWeaponHUDData();
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::OnHealthStateChanged()
{
	RefreshHealthHUDData();
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::OnInteractionStateChanged()
{
	RefreshInteractionHUDData();
	BroadcastViewModelChanged();
}
