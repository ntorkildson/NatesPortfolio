// Fill out your copyright notice in the Description page of Project Settings.


#include "WorthyAICharacter.h"
#include "UObject/Object.h"
#include "UObject/UnrealType.h"
#include "NNBrainComponent.h"
#include "Components/SphereComponent.h"
#include "GeneticAlgorithm.h"




AWorthyAICharacter::AWorthyAICharacter()
{
	// Use a sphere as a simple collision representation
	SensorRadius = CreateDefaultSubobject<USphereComponent>(TEXT("SensorRadius"));
	SensorRadius->InitSphereRadius(SenseRadius);
	SensorRadius->BodyInstance.SetCollisionProfileName("Projectile");
	SensorRadius->SetGenerateOverlapEvents(true);
	SensorRadius->InitSphereRadius(1000);
	// Players can't walk on it
	SensorRadius->CanCharacterStepUpOn = ECB_No;

	MyBrain = CreateDefaultSubobject<UNNBrainComponent>(TEXT("Brain"));
	
}

void AWorthyAICharacter::Update(TArray<FVector> readings)
{

	//create input nodes
	TArray<float> inputs;

	TArray<AActor*> nearbyGuys;
	TArray<UObject*> nearbyThings;
	SensorRadius->GetOverlappingActors(nearbyGuys);
	for (int32 i = 0; i <= nearbyGuys.Num(); i++)
	{
		float tempDistance = GetDistanceTo(nearbyGuys[i]);
		//add  vectors for closest objects(readings) to input array
		inputs.Emplace(tempDistance);

	}

	//update brain and get feedback
	//TArray<float> NNoutput = MyBrain->Update(inputs);


	//return value for whatever function we're going to call
	
	   

}



void AWorthyAICharacter::MutateMe()
{
	//send NN to GA for mutations
}

