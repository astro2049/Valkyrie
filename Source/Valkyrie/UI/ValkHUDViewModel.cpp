// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkHUDViewModel.h"

#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"

void UValkHUDViewModel::BindToWeaponComponent(UWeaponComponent* aWeaponComponent)
{
	myWeaponComponent = aWeaponComponent;
	if (myWeaponComponent) {
		myWeaponComponent->myOnWeaponStateChanged.AddUniqueDynamic(
			this,
			&UValkHUDViewModel::HandleWeaponStateChanged
		);
		HandleWeaponStateChanged();
	}
}

void UValkHUDViewModel::BindToHealthComponent(UHealthComponent* aHealthComponent)
{
	myHealthComponent = aHealthComponent;
	if (myHealthComponent) {
		myHealthComponent->OnHealthChanged.AddUniqueDynamic(
			this,
			&UValkHUDViewModel::HandleHealthChanged
		);
		HandleHealthChanged(
			myHealthComponent->GetHealth(),
			myHealthComponent->GetMaxHealth()
		);
	}
}

float UValkHUDViewModel::GetHealthRatio() const
{
	return myMaxHealth > 0.f ? myHealth / myMaxHealth : 0.f;
}

void UValkHUDViewModel::HandleWeaponStateChanged()
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

void UValkHUDViewModel::HandleHealthChanged(float aHealth, float aMaxHealth)
{
	myHealth = aHealth;
	myMaxHealth = aMaxHealth;
	myShowHealth = true;
	myIsDead = myHealth <= 0.f;
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::SetShowInteractPrompt(bool aShowInteractPrompt)
{
	myShowInteractPrompt = aShowInteractPrompt;
	BroadcastViewModelChanged();
}
