// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "ValkPlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;

UCLASS(Blueprintable)
class VALKYRIE_API AValkPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AValkPlayerCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* aPlayerInputComponent) override;
	void OnDied(AController* aDamageInstigator) const;

private:
	void HandleMove(const FInputActionValue& anInputValue);
	void HandleLook(const FInputActionValue& anInputValue);
	void HandleFire();
	void HandleReload();
	void HandleInteract();

	UFUNCTION(Server, Reliable)
	void Server_Interact();

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myMoveAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myLookAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myFireAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myReloadAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myInteractAction{nullptr};

	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UHealthComponent> myHealthComponent;
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UWeaponComponent> myWeaponComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInteractionComponent> myInteractionComponent;
};
