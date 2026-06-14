// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkViewModel.h"

void UValkViewModel::BroadcastViewModelChanged()
{
	myOnViewModelChanged.Broadcast();
}
