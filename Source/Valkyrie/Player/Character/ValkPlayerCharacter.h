// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/Interaction/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "ValkPlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UAnimMontage;

UCLASS(Blueprintable)
class VALKYRIE_API AValkPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AValkPlayerCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* aPlayerInputComponent) override;

private:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

	void HandleMove(const FInputActionValue& anInputValue);
	void HandleLook(const FInputActionValue& anInputValue);
	void HandleFire();
	void HandleReload();
	void HandleInteract();
	void HandleEquipPrimaryGun();
	void HandleEquipSecondaryGun();

	void OnDamaged(float aDamage, AController* aDamageInstigator);
	void OnDied(AController* aDamageInstigator) const;

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayHitReact();

	// input actions
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
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myPrimaryWeaponAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> mySecondaryWeaponAction{nullptr};

	// gameplay components: health, weapon, interaction
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UHealthComponent> myHealthComponent;
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UWeaponComponent> myWeaponComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInteractionComponent> myInteractionComponent;
	// hit react montage
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UAnimMontage> myHitReactMontage{nullptr};

	// ADS
	UFUNCTION(Server, Reliable, Category="Valkyrie")
	void Server_SetAiming(bool aIsAiming);
	void SetIsAiming() { Server_SetAiming(true); }
	void SetIsNotAiming() { Server_SetAiming(false); }
	void UpdateFov(float aDeltaSecond);

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myAimAction{nullptr};

	UPROPERTY(Replicated, VisibleAnywhere, Category="Valkyrie")
	bool myIsAiming{false};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myDefaultFov{90.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myAimFov{60.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myAimTransitionDuration{0.2f};
	float myAimTransitionSpeed{-1.f}; // degrees/s
	float myCurrentFov{myDefaultFov};

	// camera components
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<USpringArmComponent> mySpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UCameraComponent> myCameraComponent;
};
