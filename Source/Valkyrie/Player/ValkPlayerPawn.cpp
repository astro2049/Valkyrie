// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerPawn.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

AValkPlayerPawn::AValkPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	myInteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("myInteractionComponent"));
}

void AValkPlayerPawn::Tick(const float aDeltaSeconds)
{
	Super::Tick(aDeltaSeconds);

	if (!HasAuthority() && IsLocallyControlled()) {
		if (myShouldSyncLocationThisTick) {
			RPC_SyncLocation(GetActorLocation());
		}
		if (myShouldSyncRotationThisTick) {
			RPC_SyncRotation(GetActorRotation());
		}
	}
	myShouldSyncLocationThisTick = false;
	myShouldSyncRotationThisTick = false;
}

void AValkPlayerPawn::SetupPlayerInputComponent(UInputComponent* const aPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(aPlayerInputComponent);

	if (UEnhancedInputComponent* const enhancedInputComponent = Cast<UEnhancedInputComponent>(aPlayerInputComponent)) {
		// move input
		if (myMoveAction) {
			enhancedInputComponent->BindAction(
				myMoveAction,
				ETriggerEvent::Triggered,
				this,
				&AValkPlayerPawn::HandleMove
			);
		}
		// look input
		if (myLookAction) {
			enhancedInputComponent->BindAction(
				myLookAction,
				ETriggerEvent::Triggered,
				this,
				&AValkPlayerPawn::HandleLook
			);
		}
		// interact
		if (myInteractAction) {
			enhancedInputComponent->BindAction(
				myInteractAction,
				ETriggerEvent::Started,
				this,
				&AValkPlayerPawn::HandleInteract
			);
		}
	}
}

void AValkPlayerPawn::HandleMove(const FInputActionValue& anInputValue)
{
	if (const FVector2D moveInput = anInputValue.Get<FVector2D>(); !moveInput.IsNearlyZero()) {
		AddMovementInput(GetActorForwardVector(), moveInput.Y);
		AddMovementInput(GetActorRightVector(), moveInput.X);
		myShouldSyncLocationThisTick = true;
	}
}

void AValkPlayerPawn::HandleLook(const FInputActionValue& anInputValue)
{
	if (const FVector2D lookInput = anInputValue.Get<FVector2D>(); !lookInput.IsNearlyZero()) {
		AddControllerYawInput(lookInput.X);
		AddControllerPitchInput(lookInput.Y);
		myShouldSyncRotationThisTick = true;
	}
}

void AValkPlayerPawn::HandleInteract()
{
	if (!myInteractionComponent) {
		return;
	}

	if (!HasAuthority()) {
		RPC_Interact();
	} else {
		myInteractionComponent->Interact();
	}
}

void AValkPlayerPawn::RPC_SyncLocation_Implementation(const FVector& aLocation)
{
	SetActorLocation(aLocation, true);
}

void AValkPlayerPawn::RPC_SyncRotation_Implementation(const FRotator& aRotation)
{
	SetActorRotation(aRotation);
}

void AValkPlayerPawn::RPC_Interact_Implementation()
{
	if (myInteractionComponent) {
		myInteractionComponent->Interact();
	}
}
