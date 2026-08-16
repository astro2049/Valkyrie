// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Valkyrie/Components/HealthComponent.h"
#include "Valkyrie/Components/Interaction/InteractionComponent.h"
#include "Valkyrie/Components/WeaponComponent.h"
#include "Valkyrie/Player/GAS/AbilityInputId.h"
#include "ValkPlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UAnimMontage;
class UAimAbility;
class UDashAbility;
class UFireAbility;
class UThrowGrenadeAbility;
class UReloadAbility;
class USwitchWeaponAbility;

UCLASS(Blueprintable)
class VALKYRIE_API AValkPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AValkPlayerCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return myAsc; }
	virtual void SetupPlayerInputComponent(UInputComponent* aPlayerInputComponent) override;
	
	UFUNCTION(BlueprintPure)
	bool IsAiming() const;

private:
	// GAS
	/** Ability System Component. Required to use Gameplay Attributes and Gameplay Abilities. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Valkyrie", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAbilitySystemComponent> myAsc;
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UAimAbility> myAimAbilityType{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UReloadAbility> myReloadAbilityType{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UFireAbility> myFireAbilityType{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<USwitchWeaponAbility> mySwitchWeaponAbilityType{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UDashAbility> myDashAbilityType{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TSubclassOf<UThrowGrenadeAbility> myThrowGrenadeAbilityType{nullptr};

	// Lifecycle functions
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// user inputs
	void HandleMove(const FInputActionValue& anInputValue);
	void HandleLook(const FInputActionValue& anInputValue);
	void HandleReload();
	void HandleInteract();
	void HandleEquipPrimaryGun();
	void HandleEquipSecondaryGun();

	// gameplay events
	void OnDamaged(float aDamage, AController* aDamageInstigator);
	void OnDied(AController* aDamageInstigator) const;

	// hit react
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayHitReact();

	// ADS
	void OnAimingTagChanged(const FGameplayTag aTag, int32 aNewCount) const;
	void StartAiming() { myAsc->AbilityLocalInputPressed(EAbilityInputId::Aim); }
	void StopAiming() { myAsc->AbilityLocalInputReleased(EAbilityInputId::Aim); }
	void UpdateFov(float aDeltaSecond);
	void StartFiring() { myAsc->AbilityLocalInputPressed(EAbilityInputId::Fire); }
	void StopFiring() { myAsc->AbilityLocalInputReleased(EAbilityInputId::Fire); }
	void StartDashing() { myAsc->AbilityLocalInputPressed(EAbilityInputId::Dash); }
	void ThrowGrenade() { myAsc->AbilityLocalInputPressed(EAbilityInputId::ThrowGrenade); }

	// move speed
	void UpdateMaxMoveSpeed() const;

	// input actions
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myMoveAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myLookAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myJumpAction{nullptr};
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
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myDashAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myThrowGrenadeAction{nullptr};

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
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myAimAction{nullptr};

	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myDefaultFov{90.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myAimFov{80.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myAimTransitionDuration{0.15f};
	float myAimTransitionSpeed{-1.f}; // degrees/s
	float myCurrentFov{myDefaultFov};

	// move speed
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myDefaultMaxWalkSpeed{600.f};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	float myAimMaxWalkSpeed{400.f};

	// camera components
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<USpringArmComponent> mySpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category="Valkyrie")
	TObjectPtr<UCameraComponent> myCameraComponent;
	
	 // Dash
	UFUNCTION(BlueprintPure, Category="Valkyrie")
	bool IsDashing() const;
};
