// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Valkyrie/Actors/Gun/GunActor.h"
#include "Valkyrie/Player/Controllers/ValkPlayerController.h"

AValkPlayerCharacter::AValkPlayerCharacter()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	myHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	myWeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("myWeaponComponent"));
	myInteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("myInteractionComponent"));
}

void AValkPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (myHealthComponent) {
		myHealthComponent->OnDamaged().BindUObject(this, &AValkPlayerCharacter::OnDamaged);
		myHealthComponent->OnDied().BindUObject(this, &AValkPlayerCharacter::OnDied);
	}
}

void AValkPlayerCharacter::AttachGun(AGunActor* aGunActor)
{
	if (!aGunActor) {
		return;
	}

	if (USkeletalMeshComponent* const characterMesh = GetMesh()) {
		aGunActor->AttachToComponent(
			characterMesh,
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			myHandSocketName
		);
		aGunActor->SetOwner(this);
	}
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
		myWeaponComponent->Reload();
	}
}

void AValkPlayerCharacter::HandleInteract()
{
	Server_Interact();
}

void AValkPlayerCharacter::HandleEquipPrimaryGun()
{
	if (myWeaponComponent) {
		myWeaponComponent->EquipPrimaryGun();
	}
}

void AValkPlayerCharacter::HandleEquipSecondaryGun()
{
	if (myWeaponComponent) {
		myWeaponComponent->EquipSecondaryGun();
	}
}

void AValkPlayerCharacter::Server_Interact_Implementation()
{
	if (myInteractionComponent) {
		myInteractionComponent->Interact();
	}
}

void AValkPlayerCharacter::OnDamaged(const float, AController* const aDamageInstigator)
{
	Multicast_PlayHitReact();

	if (AValkPlayerController* const playerController = Cast<AValkPlayerController>(GetController())) {
		if (aDamageInstigator) {
			if (const APawn* const damageInstigatorPawn = aDamageInstigator->GetPawn()) {
				playerController->Client_PlayDamageRepresentations(damageInstigatorPawn->GetActorLocation());
			}
		}
	}
}

void AValkPlayerCharacter::OnDied(AController* const aDamageInstigator) const
{
	if (AValkPlayerController* const playerController = Cast<AValkPlayerController>(GetController())) {
		playerController->OnDied(aDamageInstigator);
	}
}

void AValkPlayerCharacter::Multicast_PlayHitReact_Implementation()
{
	if (myHitReactMontage) {
		PlayAnimMontage(myHitReactMontage);
	}
}
