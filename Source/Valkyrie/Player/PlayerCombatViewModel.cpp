// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCombatViewModel.h"

#include "Valkyrie/Components/WeaponComponent.h"

void UPlayerCombatViewModel::BindToWeapon(UWeaponComponent* aWeaponComponent)
{
	if (myWeaponComponent == aWeaponComponent) {
		RefreshFromWeapon();
		return;
	}

	UnbindWeapon();

	myWeaponComponent = aWeaponComponent;
	if (myWeaponComponent) {
		myWeaponComponent->myOnWeaponStateChanged.AddUniqueDynamic(
			this,
			&UPlayerCombatViewModel::HandleWeaponStateChanged
		);
	}

	RefreshFromWeapon();
}

void UPlayerCombatViewModel::UnbindWeapon()
{
	if (myWeaponComponent) {
		myWeaponComponent->myOnWeaponStateChanged.RemoveDynamic(
			this,
			&UPlayerCombatViewModel::HandleWeaponStateChanged
		);
	}

	myWeaponComponent = nullptr;
	myAmmoInMag = 0;
	myReserveAmmo = 0;
	myShowAmmo = false;
	myIsReloading = false;
	BroadcastViewModelChanged();
}

void UPlayerCombatViewModel::RefreshFromWeapon()
{
	if (!myWeaponComponent) {
		myAmmoInMag = 0;
		myReserveAmmo = 0;
		myShowAmmo = false;
		myIsReloading = false;
		BroadcastViewModelChanged();
		return;
	}

	myAmmoInMag = myWeaponComponent->GetAmmoInMag();
	myReserveAmmo = myWeaponComponent->GetReserveAmmo();
	myIsReloading = myWeaponComponent->IsReloading();
	myShowAmmo = true;
	BroadcastViewModelChanged();
}

void UPlayerCombatViewModel::HandleWeaponStateChanged()
{
	RefreshFromWeapon();
}

void UPlayerCombatViewModel::BroadcastViewModelChanged()
{
	myOnViewModelChanged.Broadcast();
}
