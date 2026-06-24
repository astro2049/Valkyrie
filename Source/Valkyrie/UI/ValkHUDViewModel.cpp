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
	return myHealthHUDData.myMaxHealth > 0.f ? myHealthHUDData.myHealth / myHealthHUDData.myMaxHealth : 0.f;
}

void UValkHUDViewModel::HandleWeaponStateChanged()
{
	if (!myWeaponComponent) {
		myWeaponHUDData = FValkWeaponHUDData{};
		BroadcastViewModelChanged();
		return;
	}

	myWeaponHUDData.myAmmoInMag = myWeaponComponent->GetAmmoInMag();
	myWeaponHUDData.myReserveAmmo = myWeaponComponent->GetReserveAmmo();
	myWeaponHUDData.myIsReloading = myWeaponComponent->IsReloading();
	myWeaponHUDData.myShowAmmo = true;
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::HandleHealthChanged(float aHealth, float aMaxHealth)
{
	myHealthHUDData.myHealth = aHealth;
	myHealthHUDData.myMaxHealth = aMaxHealth;
	myHealthHUDData.myShowHealth = true;
	myHealthHUDData.myIsDead = myHealthHUDData.myHealth <= 0.f;
	BroadcastViewModelChanged();
}

void UValkHUDViewModel::SetShowInteractPrompt(bool aShowInteractPrompt)
{
	myShowInteractPrompt = aShowInteractPrompt;
	BroadcastViewModelChanged();
}
