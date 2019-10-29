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
	//RunSimulation();

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
	if (GenerationCounter++ < LegnthOfSimulation)
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
	}
	//run GA and update brains
/*
	ThePopulationOfGenomes/Genotypes = GA->Epoch(Genotype) pass by reference I guess

	for every creature
	creatures.putweights(populationweight[i].vecweights

	creatures.reset()
*/

//average fitness
	//for FullPopulation
		//get Genotype.fitness = POpulation.Fitness
			//add them all up and average them out
			//also grab the best while we're at it
//increment generation counter
	GenerationCounter++;
	//reset timer
		//for however many 'ticks' or cycles we do this for

	/*

		run GA to create new population of GENOTYPES!
			GeneticALgorithm->EPOCH(GENOTYPES);

	*/

	//insert new brain into AI
			/*
				for each Creature
				creature.putweights(GENOTYPE[i].weights];

							//reset temp brain

				creature.reset();
			*/


	for (int32 i = 0; i <= NumberOfAI; i++)
	{
		//	FullPopulation[1].MyBrain.NeuronLayers.Neurons.weights);
		//	FullPopulation[i].Emplace(AdjustedGenomes[i].weights);
	}

}