// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WorthyGameMode.h"
#include "WorthyHUD.h"
#include "WorthyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWorthyGameMode::AWorthyGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AWorthyHUD::StaticClass();
}
