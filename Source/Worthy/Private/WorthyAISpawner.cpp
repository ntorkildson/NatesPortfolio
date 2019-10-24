// Fill out your copyright notice in the Description page of Project Settings.


#include "WorthyAISpawner.h"
#include "WorthyCharacter.h"
#include "WorthyAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GeneticAlgorithm.h"
#include "Engine/World.h"
#include "WorthyWeapon.h"

// Sets default values
AWorthyAISpawner::AWorthyAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorthyAISpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnAIClass();
	RunSimulation();

}

// Called every frame
void AWorthyAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunSimulation();
}

void AWorthyAISpawner::SpawnAIClass()
{

	//initialize GA class


	//for numberOfAI
	for (int32 i = 0; i < NumberOfAI; i++)
	{
		FVector SpawnLocation = GetOwner()->GetActorLocation()  + FVector(326, 4, 552);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		//spawn AIClass
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FTransform SpawnTransform(SpawnRotation, SpawnLocation);


		AWorthyAICharacter* NewActor = GetWorld()->SpawnActor<AWorthyAICharacter>(AIClass, ActorSpawnParams);
		//NewActor->SpawnDefaultController();

		//initialize brains
			
			//GA stats created
			

		NewActor->myStats;
		FullPopulation.Emplace(NewActor);

	}
}


void AWorthyAISpawner::RunSimulation()
{

	//run the localized simulation for each of the actors
	for (int32 i = 0; i < FullPopulation.Num(); i++)
	{
		FullPopulation[i]->UpdateBrain();

	}


	//for each AI, update NN with inputs

	//if 'events' happen adjust fitness accordingly

	//update chromosones fitness score

	//generation complete

		//run GA and update brains
	for (int32 i = 0; i < FullPopulation.Num() % 2; i++)
	{
		for (int32 j = 0; j < FullPopulation.Num() % 2; j++)
		{
			GeneticALgorithm->Epoch(FullPopulation[i], FullPopulation[j]);
			//average fitness
			//best fitness
			//increment generation counter
			GenerationCounter++;
			//reset timer

			//insert new brain into AI

			//reset temp brain
		}
	}
	for (int32 i = 0; i <= NumberOfAI; i++)
	{
		
		FullPopulation[i].Emplace(AdjustedGenomes[i].weights);
	}

}
