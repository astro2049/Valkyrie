// Fill out your copyright notice in the Description page of Project Settings.

#include "UINode_GetMaxPlayersPerTeam.h"

#include "Valkyrie/Common/ValkTypes.h"

int32 UUINode_GetMaxPlayersPerTeam::GetMaxPlayersPerTeam()
{
	return ValkGameRules::MaxPlayersPerTeam;
}
