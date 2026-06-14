// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkHUDViewModel.h"

#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"

void UValkHUDViewModel::BindToWeapon(UWeaponComponent* aWeaponComponent)
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
			&UValkHUDViewModel::HandleWeaponStateChanged
		);
	}

	RefreshFromWeapon();
}

void UValkHUDViewModel::UnbindWeapon()
{
	if (myWeaponComponent) {
		myWeaponComponent->myOnWeaponStateChanged.RemoveDynamic(
			this,
			&UValkHUDViewModel::HandleWeaponStateChanged
		);
	}

	myWeaponComponent = nullptr;
	myAmmoInMag = 0;
	myReserveAmmo = 0;
	myShowAmmo = false;
	myIsReloading = false;
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::RefreshFromWeapon()
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

void UValkHUDViewModel::BindToHealth(UHealthComponent* aHealthComponent)
{
	if (myHealthComponent == aHealthComponent) {
		RefreshFromHealth();
		return;
	}

	UnbindHealth();

	myHealthComponent = aHealthComponent;
	if (myHealthComponent) {
		myHealthComponent->OnHealthChanged.AddUniqueDynamic(
			this,
			&UValkHUDViewModel::HandleHealthChanged
		);
		myHealthComponent->OnDeath.AddUniqueDynamic(
			this,
			&UValkHUDViewModel::HandleDeath
		);
	}

	RefreshFromHealth();
}

void UValkHUDViewModel::UnbindHealth()
{
	if (myHealthComponent) {
		myHealthComponent->OnHealthChanged.RemoveDynamic(
			this,
			&UValkHUDViewModel::HandleHealthChanged
		);
		myHealthComponent->OnDeath.RemoveDynamic(
			this,
			&UValkHUDViewModel::HandleDeath
		);
	}

	myHealthComponent = nullptr;
	myHealth = 0.f;
	myMaxHealth = 0.f;
	myShowHealth = false;
	myIsDead = false;
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::RefreshFromHealth()
{
	if (!myHealthComponent) {
		myHealth = 0.f;
		myMaxHealth = 0.f;
		myShowHealth = false;
		myIsDead = false;
		BroadcastViewModelChanged();
		return;
	}

	myHealth = myHealthComponent->GetHealth();
	myMaxHealth = myHealthComponent->GetMaxHealth();
	myShowHealth = true;
	myIsDead = myHealthComponent->IsDead();
	BroadcastViewModelChanged();
}

float UValkHUDViewModel::GetHealthRatio() const
{
	return myMaxHealth > 0.f ? myHealth / myMaxHealth : 0.f;
}

void UValkHUDViewModel::HandleWeaponStateChanged()
{
	RefreshFromWeapon();
}

void UValkHUDViewModel::HandleHealthChanged(float aHealth, float aMaxHealth)
{
	myHealth = aHealth;
	myMaxHealth = aMaxHealth;
	myShowHealth = true;
	myIsDead = myHealth <= 0.f;
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::HandleDeath()
{
	myHealth = 0.f;
	myIsDead = true;
	BroadcastViewModelChanged();
}
