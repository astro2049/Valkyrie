// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkPlayerState.h"

#include "Net/UnrealNetwork.h"

void AValkPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AValkPlayerState, myTeamId);
}

void AValkPlayerState::CopyProperties(APlayerState* const aPlayerState)
{
	Super::CopyProperties(aPlayerState);

	if (AValkPlayerState* const newPlayerState = Cast<AValkPlayerState>(aPlayerState)) {
		newPlayerState->SetTeamId(myTeamId);
	}
}
