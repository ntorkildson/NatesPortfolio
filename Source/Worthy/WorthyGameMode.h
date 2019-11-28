// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WorthyGameMode.generated.h"


UENUM(BlueprintType)
enum class EGameState : uint8
{
	PlanningPhase	UMETA(DisplayName = "Planning Phase"),
	ExecutionPhase	UMETA(DisplayName = "ExecutionPhase" ),
	RecapPhase		UMETA(DisplayName = "Recap PHase"), 
	ExplorePhase	UMETA(DisplayName = "Explore Phase" ),

};

UCLASS(minimalapi)
class AWorthyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWorthyGameMode();

	EGameState CurrentState;

	void StartCombat();

	void SetPlanningPhase();

	int32 PlanningPhaseLength;

	FTimerHandle PlanningPhaseTimer;


	void SetExecutionPhase();

	int32 ExecutionPhaseLength;

	FTimerHandle ExecitopmPhaseTimer;

	void SetRecapPhase();

	int32 RecapPhaseLength;

	FTimerHandle RecapPhaseTimer;

	void SetWanderPhase();

	void CreateCombatZone();




};



