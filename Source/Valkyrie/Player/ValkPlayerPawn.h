// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Valkyrie/Components/InteractionComponent.h"
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

protected:
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie|Input")
	TObjectPtr<UInputAction> myMoveAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie|Input")
	TObjectPtr<UInputAction> myLookAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie|Input")
	TObjectPtr<UInputAction> myInteractAction{nullptr};

private:
	void HandleMove(const FInputActionValue& anInputValue);
	void HandleLook(const FInputActionValue& anInputValue);
	void HandleInteract();

	UFUNCTION(Server, Unreliable)
	void Server_SyncLocation(FVector aLocation);
	UFUNCTION(Server, Unreliable)
	void Server_SyncRotation(FRotator aRotation);

	bool myShouldSyncLocationThisTick{false};
	bool myShouldSyncRotationThisTick{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie|Interaction", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInteractionComponent> myInteractionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie|Interaction", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USphereComponent> mySphereComponent;
};
