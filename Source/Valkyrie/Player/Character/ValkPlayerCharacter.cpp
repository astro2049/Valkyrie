// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Valkyrie/GameModes/ValkGameMode.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "Valkyrie/Player/GAS/AbilityInputId.h"
#include "Valkyrie/Player/GAS/AbilityTags.h"
#include "Valkyrie/Player/GAS/GameplayAbilities/AimAbility.h"
#include "Valkyrie/Player/GAS/GameplayAbilities/DashAbility.h"
#include "Valkyrie/Player/GAS/GameplayAbilities/FireAbility.h"
#include "Valkyrie/Player/GAS/GameplayAbilities/ThrowGrenadeAbility.h"
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

	myHealthComponent->GetOnDamaged().BindUObject(this, &AValkPlayerCharacter::OnDamaged);
	myHealthComponent->GetOnDied().BindUObject(this, &AValkPlayerCharacter::OnDied);
	myAimTransitionSpeed = (myAimFov - myDefaultFov) / myAimTransitionDuration;
	UpdateMaxMoveSpeed();

	if (HasAuthority()) {
		myAsc->GiveAbility(FGameplayAbilitySpec(myAimAbilityType, 1, EAbilityInputId::Aim));
		myAsc->GiveAbility(FGameplayAbilitySpec(myReloadAbilityType, 1, EAbilityInputId::Reload));
		myAsc->GiveAbility(FGameplayAbilitySpec(myFireAbilityType, 1, EAbilityInputId::Fire));
		myAsc->GiveAbility(FGameplayAbilitySpec(mySwitchWeaponAbilityType, 1, EAbilityInputId::PrimaryWeapon));
		myAsc->GiveAbility(FGameplayAbilitySpec(mySwitchWeaponAbilityType, 1, EAbilityInputId::SecondaryWeapon));
		myAsc->GiveAbility(FGameplayAbilitySpec(myDashAbilityType, 1, EAbilityInputId::Dash));
		myAsc->GiveAbility(FGameplayAbilitySpec(myThrowGrenadeAbilityType, 1, EAbilityInputId::ThrowGrenade));
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

	UEnhancedInputComponent* const enhancedInputComponent = CastChecked<UEnhancedInputComponent>(aPlayerInputComponent);
	enhancedInputComponent->BindAction(
		myMoveAction,
		ETriggerEvent::Triggered,
		this,
		&AValkPlayerCharacter::HandleMove
	);
	enhancedInputComponent->BindAction(
		myLookAction,
		ETriggerEvent::Triggered,
		this,
		&AValkPlayerCharacter::HandleLook
	);
	enhancedInputComponent->BindAction(
		myJumpAction,
		ETriggerEvent::Started,
		this,
		&ACharacter::Jump
	);
	enhancedInputComponent->BindAction(
		myJumpAction,
		ETriggerEvent::Completed,
		this,
		&ACharacter::StopJumping
	);
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
	enhancedInputComponent->BindAction(
		myReloadAction,
		ETriggerEvent::Started,
		this,
		&AValkPlayerCharacter::HandleReload
	);
	enhancedInputComponent->BindAction(
		myInteractAction,
		ETriggerEvent::Started,
		this,
		&AValkPlayerCharacter::HandleInteract
	);
	enhancedInputComponent->BindAction(
		myPrimaryWeaponAction,
		ETriggerEvent::Started,
		this,
		&AValkPlayerCharacter::HandleEquipPrimaryGun
	);
	enhancedInputComponent->BindAction(
		mySecondaryWeaponAction,
		ETriggerEvent::Started,
		this,
		&AValkPlayerCharacter::HandleEquipSecondaryGun
	);
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
	enhancedInputComponent->BindAction(
		myDashAction,
		ETriggerEvent::Started,
		this,
		&AValkPlayerCharacter::StartDashing
	);
	enhancedInputComponent->BindAction(
		myThrowGrenadeAction,
		ETriggerEvent::Started,
		this,
		&AValkPlayerCharacter::ThrowGrenade
	);
}

void AValkPlayerCharacter::HandleMove(const FInputActionValue& anInputValue)
{
	if (const FVector2D moveInput = anInputValue.Get<FVector2D>(); !moveInput.IsNearlyZero()) {
		const FRotator controlRotation = GetControlRotation();
		const FRotator yawRotation(0.f, controlRotation.Yaw, 0.f);
		AddMovementInput(FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X), moveInput.Y);
		AddMovementInput(FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y), moveInput.X);
	}
}

void AValkPlayerCharacter::HandleLook(const FInputActionValue& anInputValue)
{
	if (const FVector2D lookInput = anInputValue.Get<FVector2D>(); !lookInput.IsNearlyZero()) {
		AddControllerYawInput(lookInput.X);
		AddControllerPitchInput(lookInput.Y);
	}
}

void AValkPlayerCharacter::HandleInteract()
{
	myInteractionComponent->Server_Interact();
}

void AValkPlayerCharacter::OnDamaged(const float, AController* const anAttacker)
{
	// play hit react montage
	Multicast_PlayHitReact();

	// play damage indicator
	if (anAttacker) {
		AValkPlayerController* const playerController = CastChecked<AValkPlayerController>(GetController());
		if (const APawn* const attackerPawn = anAttacker->GetPawn()) {
			playerController->Client_PlayDamageRepresentations(attackerPawn->GetActorLocation());
		}
	}
}

void AValkPlayerCharacter::Multicast_PlayHitReact_Implementation()
{
	PlayAnimMontage(myHitReactMontage);
}

void AValkPlayerCharacter::OnDied(AController* const anAttacker)
{
	AController* const controller = GetController();
	FVector attackerLocation = GetActorLocation();
	if (anAttacker) {
		if (const APawn* const attackerPawn = anAttacker->GetPawn()) {
			attackerLocation = attackerPawn->GetActorLocation();
		}
	}
	Multicast_PlayDeathPresentation(attackerLocation);
	CastChecked<AValkPlayerController>(controller)->Client_OnPlayerDied();
	GetWorld()->GetAuthGameMode<AValkGameMode>()->PlayerDied(anAttacker, controller);
}

void AValkPlayerCharacter::Multicast_PlayDeathPresentation_Implementation(const FVector anAttackerLocation)
{
	const FVector deathImpulse = (GetActorLocation() - anAttackerLocation).GetSafeNormal() * myDeathImpulseStrength;
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->AddImpulse(deathImpulse, TEXT("spine_05"), true);
}

void AValkPlayerCharacter::UpdateFov(const float aDeltaSecond)
{
	const float fovOffset = aDeltaSecond * (IsAiming() ? myAimTransitionSpeed : -myAimTransitionSpeed);
	myCurrentFov = FMath::Clamp(myCurrentFov + fovOffset, myAimFov, myDefaultFov);

	myCameraComponent->SetFieldOfView(myCurrentFov);
}
