// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "WeaponComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VALKYRIE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void Fire();
	UFUNCTION(BlueprintCallable, Category="Valkyrie")
	void StartReload();

	int32 GetAmmoInMag() const { return myAmmoInMag; }
	int32 GetReserveAmmo() const { return myReserveAmmo; }
	int32 GetMagazineSize() const { return myMagazineSize; }
	bool IsReloading() const { return myIsReloading; }

private:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_TraceFire(FVector aTraceStart, FVector aTraceDirection);
	UFUNCTION(Server, Reliable)
	void Server_TryStartReload();
	void FinishReload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myDamage{25.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myRPM{600.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	int32 myMagazineSize{30};
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	int32 myAmmoInMag{30};
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	int32 myReserveAmmo{90};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myReloadDuration{2.f};
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	bool myIsReloading{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	float myTraceDistance{1000.f};

	float myFireInterval{-1.f};
	float myLastFiredTime{-1.f};
	FTimerHandle myReloadTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	USoundBase* myFireSound{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Valkyrie", meta=(AllowPrivateAccess="true"))
	bool myDrawDebugTrace{true};
};
