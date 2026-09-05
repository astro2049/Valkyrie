// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_HUD.h"

void UUI_HUD::ToggleInputActionsMenu()
{
	if (!myIsShowingInputActionsMenu) {
		ShowInputActionsMenu();
	} else {
		HideInputActionsMenu();
	}
	myIsShowingInputActionsMenu = !myIsShowingInputActionsMenu;
}
