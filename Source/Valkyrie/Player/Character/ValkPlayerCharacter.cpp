// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "Valkyrie/Player/GAS/AbilityInputId.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"
#include "Valkyrie/Player/GAS/GameplayAbilities/AimAbility.h"
#include "Valkyrie/Player/GAS/GameplayAbilities/FireAbility.h"
#include "Valkyrie/Player/GAS/GameplayAbilities/ReloadAbility.h"
#include "Valkyrie/Player/GAS/GameplayAbilities/SwitchWeaponAbility.h"

AValkPlayerCharacter::AValkPlayerCharacter()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	myHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	myWeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("myWeaponComponent"));
	myInteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("myInteractionComponent"));
	mySpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("mySpringArmComponent"));
	mySpringArmComponent->SetupAttachment(GetRootComponent());
	mySpringArmComponent->bUsePawnControlRotation = true;
	myCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("myCameraComponent"));
	myCameraComponent->SetupAttachment(mySpringArmComponent, USpringArmComponent::SocketName);

	myAsc = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("myAbilitySystemComponent"));
	myAsc->SetIsReplicated(true);
}

void AValkPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (myHealthComponent) {
		myHealthComponent->GetOnDamaged().BindUObject(this, &AValkPlayerCharacter::OnDamaged);
		myHealthComponent->GetOnDied().BindUObject(this, &AValkPlayerCharacter::OnDied);
	}
	myAimTransitionSpeed = (myAimFov - myDefaultFov) / myAimTransitionDuration;
	UpdateMaxMoveSpeed();

	if (HasAuthority()) {
		if (ensureMsgf(myAimAbilityType != nullptr, TEXT("%s has no Aim ability type assigned."), *GetNameSafe(this))) {
			myAsc->GiveAbility(FGameplayAbilitySpec(myAimAbilityType, 1, EAbilityInputId::Aim));
		}
		if (ensureMsgf(myReloadAbilityType != nullptr, TEXT("%s has no Reload ability type assigned."), *GetNameSafe(this))) {
			myAsc->GiveAbility(FGameplayAbilitySpec(myReloadAbilityType, 1, EAbilityInputId::Reload));
		}
		if (ensureMsgf(myFireAbilityType != nullptr, TEXT("%s has no Fire ability type assigned."), *GetNameSafe(this))) {
			myAsc->GiveAbility(FGameplayAbilitySpec(myFireAbilityType, 1, EAbilityInputId::Fire));
		}
		if (ensureMsgf(mySwitchWeaponAbilityType != nullptr, TEXT("%s has no Switch Weapon ability type assigned."), *GetNameSafe(this))) {
			myAsc->GiveAbility(FGameplayAbilitySpec(mySwitchWeaponAbilityType, 1, EAbilityInputId::PrimaryWeapon));
			myAsc->GiveAbility(FGameplayAbilitySpec(mySwitchWeaponAbilityType, 1, EAbilityInputId::SecondaryWeapon));
		}
	}

	myAsc->RegisterGameplayTagEvent(
		AbilityTags::Ability_Aim,
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(
		this,
		&AValkPlayerCharacter::OnAimingTagChanged
	);
}

void AValkPlayerCharacter::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateFov(DeltaSeconds);
}

void AValkPlayerCharacter::SetupPlayerInputComponent(UInputComponent* const aPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(aPlayerInputComponent);

	if (UEnhancedInputComponent* const enhancedInputComponent = Cast<UEnhancedInputComponent>(aPlayerInputComponent)) {
		if (myMoveAction) {
			enhancedInputComponent->BindAction(
				myMoveAction,
				ETriggerEvent::Triggered,
				this,
				&AValkPlayerCharacter::HandleMove
			);
		}
		if (myLookAction) {
			enhancedInputComponent->BindAction(
				myLookAction,
				ETriggerEvent::Triggered,
				this,
				&AValkPlayerCharacter::HandleLook
			);
		}
		if (myFireAction) {
			enhancedInputComponent->BindAction(
				myFireAction,
				ETriggerEvent::Started,
				this,
				&AValkPlayerCharacter::StartFiring
			);
			enhancedInputComponent->BindAction(
				myFireAction,
				ETriggerEvent::Completed,
				this,
				&AValkPlayerCharacter::StopFiring
			);
		}
		if (myReloadAction) {
			enhancedInputComponent->BindAction(
				myReloadAction,
				ETriggerEvent::Started,
				this,
				&AValkPlayerCharacter::HandleReload
			);
		}
		if (myInteractAction) {
			enhancedInputComponent->BindAction(
				myInteractAction,
				ETriggerEvent::Started,
				this,
				&AValkPlayerCharacter::HandleInteract
			);
		}
		if (myPrimaryWeaponAction) {
			enhancedInputComponent->BindAction(
				myPrimaryWeaponAction,
				ETriggerEvent::Started,
				this,
				&AValkPlayerCharacter::HandleEquipPrimaryGun
			);
		}
		if (mySecondaryWeaponAction) {
			enhancedInputComponent->BindAction(
				mySecondaryWeaponAction,
				ETriggerEvent::Started,
				this,
				&AValkPlayerCharacter::HandleEquipSecondaryGun
			);
		}
		if (myAimAction) {
			enhancedInputComponent->BindAction(
				myAimAction,
				ETriggerEvent::Started,
				this,
				&AValkPlayerCharacter::StartAiming
			);
			enhancedInputComponent->BindAction(
				myAimAction,
				ETriggerEvent::Completed,
				this,
				&AValkPlayerCharacter::StopAiming
			);
		}
	}
}

void AValkPlayerCharacter::HandleMove(const FInputActionValue& anInputValue)
{
	if (const FVector2D moveInput = anInputValue.Get<FVector2D>(); !moveInput.IsNearlyZero()) {
		if (const AController* const controller = GetController()) {
			const FRotator controlRotation = controller->GetControlRotation();
			const FRotator yawRotation(0.f, controlRotation.Yaw, 0.f);
			AddMovementInput(FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X), moveInput.Y);
			AddMovementInput(FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y), moveInput.X);
		}
	}
}

void AValkPlayerCharacter::HandleLook(const FInputActionValue& anInputValue)
{
	if (const FVector2D lookInput = anInputValue.Get<FVector2D>(); !lookInput.IsNearlyZero()) {
		AddControllerYawInput(lookInput.X);
		AddControllerPitchInput(lookInput.Y);
	}
}

void AValkPlayerCharacter::HandleReload()
{
	myAsc->AbilityLocalInputPressed(EAbilityInputId::Reload);
}

void AValkPlayerCharacter::HandleInteract()
{
	if (myInteractionComponent) {
		myInteractionComponent->Server_Interact();
	}
}

void AValkPlayerCharacter::HandleEquipPrimaryGun()
{
	if (myWeaponComponent && myWeaponComponent->GetCurrentSlot() != EValkWeaponSlot::Primary) {
		myAsc->AbilityLocalInputPressed(EAbilityInputId::PrimaryWeapon);
	}
}

void AValkPlayerCharacter::HandleEquipSecondaryGun()
{
	if (myWeaponComponent && myWeaponComponent->GetCurrentSlot() != EValkWeaponSlot::Secondary) {
		myAsc->AbilityLocalInputPressed(EAbilityInputId::SecondaryWeapon);
	}
}

void AValkPlayerCharacter::OnDamaged(const float, AController* const aDamageInstigator)
{
	// play hit react montage
	Multicast_PlayHitReact();

	// play damage indicator
	if (aDamageInstigator) {
		AValkPlayerController* const playerController = Cast<AValkPlayerController>(GetController());
		const APawn* const damageInstigatorPawn = aDamageInstigator->GetPawn();
		if (playerController && damageInstigatorPawn) {
			playerController->Client_PlayDamageRepresentations(damageInstigatorPawn->GetActorLocation());
		}
	}
}

void AValkPlayerCharacter::Multicast_PlayHitReact_Implementation()
{
	if (myHitReactMontage) {
		PlayAnimMontage(myHitReactMontage);
	}
}

void AValkPlayerCharacter::OnDied(AController* const aDamageInstigator) const
{
	if (AValkPlayerController* const playerController = Cast<AValkPlayerController>(GetController())) {
		playerController->OnControlledPawnDied(aDamageInstigator);
	}
}
bool AValkPlayerCharacter::IsAiming() const
{
	return myAsc->HasMatchingGameplayTag(AbilityTags::Ability_Aim);
}

void AValkPlayerCharacter::OnAimingTagChanged(const FGameplayTag aTag, int32 aNewCount) const
{
	UpdateMaxMoveSpeed();
}

void AValkPlayerCharacter::UpdateFov(const float aDeltaSecond)
{
	const float fovOffset = aDeltaSecond * (IsAiming() ? myAimTransitionSpeed : -myAimTransitionSpeed);
	myCurrentFov = FMath::Clamp(myCurrentFov + fovOffset, myAimFov, myDefaultFov);

	myCameraComponent->SetFieldOfView(myCurrentFov);
}

void AValkPlayerCharacter::UpdateMaxMoveSpeed() const
{
	GetCharacterMovement()->MaxWalkSpeed = IsAiming() ? myAimMaxWalkSpeed : myDefaultMaxWalkSpeed;
}
