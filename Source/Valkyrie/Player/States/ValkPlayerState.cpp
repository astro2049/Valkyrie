// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerState.h"

#include "Net/UnrealNetwork.h"
void AValkPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AValkPlayerState, myTeamId);
}
