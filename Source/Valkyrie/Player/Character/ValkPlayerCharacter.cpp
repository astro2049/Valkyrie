// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"
#include "Valkyrie/Player/GAS/AbilityInputId.h"
#include "Valkyrie/Player/GAS/GameplayAbilities/AimAbility.h"

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
		myAsc->GiveAbility(FGameplayAbilitySpec(UAimAbility::StaticClass(), 1, EAbilityInputId::Aim));
	}
}

void AValkPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AValkPlayerCharacter, myIsAiming);
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
				ETriggerEvent::Triggered,
				this,
				&AValkPlayerCharacter::HandleFire
			);
		}
		if (myReloadAction) {
			enhancedInputComponent->BindAction(
				myReloadAction,
				ETriggerEvent::Triggered,
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

void AValkPlayerCharacter::HandleFire()
{
	if (myWeaponComponent) {
		myWeaponComponent->Fire();
	}
}

void AValkPlayerCharacter::HandleReload()
{
	if (myWeaponComponent) {
		myWeaponComponent->Server_Reload();
	}
}

void AValkPlayerCharacter::HandleInteract()
{
	if (myInteractionComponent) {
		myInteractionComponent->Server_Interact();
	}
}

void AValkPlayerCharacter::HandleEquipPrimaryGun()
{
	if (myWeaponComponent) {
		myWeaponComponent->Server_EquipGun(EValkWeaponSlot::Primary);
	}
}

void AValkPlayerCharacter::HandleEquipSecondaryGun()
{
	if (myWeaponComponent) {
		myWeaponComponent->Server_EquipGun(EValkWeaponSlot::Secondary);
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

void AValkPlayerCharacter::SetAiming(const bool aIsAiming)
{
	myIsAiming = aIsAiming;
	UpdateMaxMoveSpeed();
}

void AValkPlayerCharacter::UpdateFov(const float aDeltaSecond)
{
	const float fovOffset = aDeltaSecond * (myIsAiming ? myAimTransitionSpeed : -myAimTransitionSpeed);
	myCurrentFov = FMath::Clamp(myCurrentFov + fovOffset, myAimFov, myDefaultFov);

	myCameraComponent->SetFieldOfView(myCurrentFov);
}

void AValkPlayerCharacter::OnRep_IsAiming() const
{
	UpdateMaxMoveSpeed();
}

void AValkPlayerCharacter::UpdateMaxMoveSpeed() const
{
	GetCharacterMovement()->MaxWalkSpeed = myIsAiming ? myAimMaxWalkSpeed : myDefaultMaxWalkSpeed;
}
