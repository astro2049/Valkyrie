// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Valkyrie/Components/InteractionComponent.h"
#include "ValkPlayerPawn.generated.h"

struct FInputActionValue;
class UInputAction;

DECLARE_MULTICAST_DELEGATE(FValkPlayerPawnStateChanged);

UCLASS(Blueprintable)
class VALKYRIE_API AValkPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AValkPlayerPawn();

	virtual void Tick(float aDeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* aPlayerInputComponent) override;

	FValkPlayerPawnStateChanged myOnPlayerPawnStateChanged; // for HUD view model

protected:
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myMoveAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myLookAction{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="Valkyrie")
	TObjectPtr<UInputAction> myInteractAction{nullptr};

private:
	virtual void BeginPlay() override;

	void HandleMove(const FInputActionValue& anInputValue);
	void HandleLook(const FInputActionValue& anInputValue);
	void HandleInteract();
	void HandleComponentStateChanged() const;

	UFUNCTION(Server, Unreliable)
	void RPC_SyncLocation(const FVector& aLocation);
	UFUNCTION(Server, Unreliable)
	void RPC_SyncRotation(const FRotator& aRotation);
	UFUNCTION(Server, Reliable)
	void RPC_Interact();

	bool myShouldSyncLocationThisTick{false};
	bool myShouldSyncRotationThisTick{false};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInteractionComponent> myInteractionComponent;
};
