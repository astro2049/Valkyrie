// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetHUDData.h"

#include "GameFramework/PlayerController.h"
#include "Valkyrie/Player/ValkPlayerCharacter.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"

FValkHUDData UUINode_GetHUDData::GetHUDData(APlayerController* const aPlayerController)
{
	FValkHUDData data;
	if (!aPlayerController) {
		return data;
	}

	if (const AValkPlayerCharacter* const playerCharacter = Cast<AValkPlayerCharacter>(aPlayerController->GetPawn())) {
		if (const UHealthComponent* const healthComponent = playerCharacter->FindComponentByClass<UHealthComponent>()) {
			data.myHealth = healthComponent->GetHealth();
			data.myMaxHealth = healthComponent->GetMaxHealth();
		}
		if (const UWeaponComponent* const weaponComponent = playerCharacter->FindComponentByClass<UWeaponComponent>()) {
			data.myAmmo = weaponComponent->GetAmmoInMag();
			data.myReserveAmmo = weaponComponent->GetReserveAmmo();
			data.myIsReloading = weaponComponent->IsReloading();
		}
		if (const UInteractionComponent* const interactionComponent = playerCharacter->FindComponentByClass<UInteractionComponent>()) {
			data.myHasInteractable = interactionComponent->HasInteractable();
		}
	}
	return data;
}
