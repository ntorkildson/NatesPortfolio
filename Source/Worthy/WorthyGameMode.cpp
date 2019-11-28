// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WorthyGameMode.h"
#include "WorthyHUD.h"
#include "WorthyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"



AWorthyGameMode::AWorthyGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AWorthyHUD::StaticClass();
}



void AWorthyGameMode::StartCombat()
{
	CreateCombatZone();
	SetPlanningPhase();
}

void AWorthyGameMode::SetPlanningPhase()
{
	//start timer for planning phase end
	GetWorld()->GetTimerManager().SetTimer(PlanningPhaseTimer, this, &AWorthyGameMode::SetExecutionPhase, PlanningPhaseLength , true);

	//have player create a queue of actions
	//for each player/AI verify the queue  and store it locally
	//
}

void AWorthyGameMode::SetExecutionPhase()
{

}

void AWorthyGameMode::SetRecapPhase()
{

}

void AWorthyGameMode::SetWanderPhase()
{
	//default non combat rules exist
}

void AWorthyGameMode::CreateCombatZone()
{

}

