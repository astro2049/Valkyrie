// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::Fire()
{
	UWorld* world = GetWorld();
	APawn* ownerPawn = Cast<APawn>(GetOwner());
	if (!world || !ownerPawn) {
		return;
	}

	APlayerController* playerController = Cast<APlayerController>(ownerPawn->GetController());
	if (!playerController || !playerController->PlayerCameraManager) {
		return;
	}

	const float now = world->GetTimeSeconds();
	const float fireInterval = myRPM > 0.f ? 60.f / myRPM : 0.f;
	if (now - myLastFiredTime < fireInterval) {
		return;
	}

	myLastFiredTime = now;

	const FVector start = playerController->PlayerCameraManager->GetCameraLocation();
	const FVector direction = playerController->PlayerCameraManager->GetActorForwardVector();
	const FVector end = start + direction * myTraceDistance;
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(ownerPawn);
	const bool bHit = world->LineTraceSingleByChannel(
		hitResult,
		start,
		end,
		ECC_Visibility,
		params
	);

	if (myDrawDebugTrace) {
		DrawDebugLine(
			world,
			start,
			bHit ? hitResult.ImpactPoint : end,
			bHit ? FColor::Green : FColor::Red,
			false,
			1.0f,
			0,
			1.5f
		);
	}

	if (myFireSound) {
		UGameplayStatics::PlaySoundAtLocation(world, myFireSound, start);
	}

	if (bHit) {
		if (AActor* hitActor = hitResult.GetActor()) {
			if (UHealthComponent* health = hitActor->FindComponentByClass<UHealthComponent>()) {
				health->ApplyDamage(myDamage);
			}

			myOnHitActor.Broadcast(hitActor, myDamage);
		}
	}
}
