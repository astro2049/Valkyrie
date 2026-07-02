// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "ValkPlayerPawn.generated.h"

struct FInputActionValue;
class UInputAction;

UCLASS(Blueprintable)
class VALKYRIE_API AValkPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AValkPlayerPawn();

	virtual void Tick(float aDeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* aPlayerInputComponent) override;

private:
	void HandleMove(const FInputActionValue& anInputValue);
	void HandleLook(const FInputActionValue& anInputValue);
	void HandleFire();
	void HandleReload();
	void HandleInteract();

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

	UFUNCTION(Server, Unreliable)
	void Server_SyncLocation(const FVector& aLocation);
	UFUNCTION(Server, Unreliable)
	void Server_SyncRotation(const FRotator& aRotation);
	UFUNCTION(Server, Reliable)
	void Server_Interact();

	bool myShouldSyncLocationThisTick{false};
	bool myShouldSyncRotationThisTick{false};

	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UHealthComponent> myHealthComponent;
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UWeaponComponent> myWeaponComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInteractionComponent> myInteractionComponent;
};
