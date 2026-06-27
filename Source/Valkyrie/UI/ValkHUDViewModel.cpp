// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkHUDViewModel.h"

#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/Player/ValkPlayerPawn.h"

void UValkHUDViewModel::BindToPawn(AValkPlayerPawn* aPlayerPawn)
{
	if (myPlayerPawn.Get() == aPlayerPawn) {
		RefreshFromPawn();
		return;
	}

	myPlayerPawn = aPlayerPawn;
	if (myPlayerPawn.IsValid()) {
		if (UWeaponComponent* weaponComponent = myPlayerPawn->FindComponentByClass<UWeaponComponent>()) {
			weaponComponent->myOnWeaponStateChanged.AddUObject(
				this,
				&UValkHUDViewModel::HandleWeaponStateChanged
			);
		}

		if (UHealthComponent* healthComponent = myPlayerPawn->FindComponentByClass<UHealthComponent>()) {
			healthComponent->myOnHealthStateChanged.AddUObject(
				this,
				&UValkHUDViewModel::HandleHealthStateChanged
			);
		}

		if (UInteractionComponent* interactionComponent = myPlayerPawn->FindComponentByClass<UInteractionComponent>()) {
			interactionComponent->myOnInteractionStateChanged.AddUObject(
				this,
				&UValkHUDViewModel::HandleInteractionStateChanged
			);
		}
	}

	RefreshFromPawn();
}

void UValkHUDViewModel::RefreshFromPawn()
{
	RefreshWeaponHUDData();
	RefreshHealthHUDData();
	RefreshInteractionHUDData();
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::RefreshWeaponHUDData()
{
	UWeaponComponent* weaponComponent = myPlayerPawn.IsValid()
		? myPlayerPawn->FindComponentByClass<UWeaponComponent>()
		: nullptr;
	if (weaponComponent) {
		myWeaponHUDData.myAmmoInMag = weaponComponent->GetAmmoInMag();
		myWeaponHUDData.myReserveAmmo = weaponComponent->GetReserveAmmo();
		myWeaponHUDData.myIsReloading = weaponComponent->IsReloading();
		myWeaponHUDData.myShowAmmo = true;
	} else {
		myWeaponHUDData = FValkWeaponHUDData{};
	}
}

void UValkHUDViewModel::RefreshHealthHUDData()
{
	UHealthComponent* healthComponent = myPlayerPawn.IsValid()
		? myPlayerPawn->FindComponentByClass<UHealthComponent>()
		: nullptr;
	if (healthComponent) {
		myHealthHUDData.myHealth = healthComponent->GetHealth();
		myHealthHUDData.myMaxHealth = healthComponent->GetMaxHealth();
		myHealthHUDData.myShowHealth = true;
		myHealthHUDData.myIsDead = healthComponent->IsDead();
	} else {
		myHealthHUDData = FValkHealthHUDData{};
	}
}

void UValkHUDViewModel::RefreshInteractionHUDData()
{
	UInteractionComponent* interactionComponent = myPlayerPawn.IsValid()
		? myPlayerPawn->FindComponentByClass<UInteractionComponent>()
		: nullptr;
	myShowInteractPrompt = interactionComponent && interactionComponent->HasInteractable();
}

void UValkHUDViewModel::HandleWeaponStateChanged()
{
	RefreshWeaponHUDData();
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::HandleHealthStateChanged()
{
	RefreshHealthHUDData();
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::HandleInteractionStateChanged()
{
	RefreshInteractionHUDData();
	BroadcastViewModelChanged();
}

float UValkHUDViewModel::GetHealthRatio() const
{
	return myHealthHUDData.myMaxHealth > 0.f ? myHealthHUDData.myHealth / myHealthHUDData.myMaxHealth : 0.f;
}
