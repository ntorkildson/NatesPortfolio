// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneticAlgorithm.h"
#include "WorthyCharacter.h"

// Sets default values
AGeneticAlgorithm::AGeneticAlgorithm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



float AGeneticAlgorithm::AssignFitness()
{
	return 0; 
	//this can literally be whatever we want, and should be dependant on the individual needs
}

void AGeneticAlgorithm::RouletteSelection()
{
	//for each item in whatever class
	//randomly decide if it lives or dies
	//take remaining and breed together randomly
	//its really not a great way to do things tho....
}

void AGeneticAlgorithm::Adaptation()
{
	//TODO
}

void AGeneticAlgorithm::Coevolution()
{
	//TODO

}



FGenotype AGeneticAlgorithm::BabyMaker(FGenotype& mom, FGenotype& dad)
{
	FGenotype child;

	//this is clunky...
	child.Dex = CrossOver(mom.Dex, dad.Dex);
	child.Dex = Mutation(child.Dex, child.mutationRate, child.maxPertubation);
	

	return child;
}



// Called when the game starts or when spawned
void AGeneticAlgorithm::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGeneticAlgorithm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
