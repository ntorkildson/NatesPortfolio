// Fill out your copyright notice in the Description page of Project Settings.


#include "WorthyAICharacter.h"
#include "NNBrainComponent.h"
#include "GeneticAlgorithm.h"
#include "Components/SphereComponent.h"
#include "UnrealType.h"
#include "DrawDebugHelpers.h"



AWorthyAICharacter::AWorthyAICharacter()
{
	// Use a sphere as a simple collision representation
	PerceptionRange = CreateDefaultSubobject<USphereComponent>(TEXT("PerceptionRange"));
	PerceptionRange->InitSphereRadius(SenseRadius);
	PerceptionRange->BodyInstance.SetCollisionProfileName("Projectile");
	PerceptionRange->SetGenerateOverlapEvents(true);
	PerceptionRange->InitSphereRadius(1000);
	// Players can't walk on it
	PerceptionRange->CanCharacterStepUpOn = ECB_No;

	MyBrain = CreateDefaultSubobject<UNNBrainComponent>(TEXT("Brain"));

}

void AWorthyAICharacter::BeginPlay()
{
	Super::BeginPlay();
	MyBrain->CreateNetwork();
}



void AWorthyAICharacter::Tick(float DeltaTime)
{

}

void AWorthyAICharacter::UpdateBrain()
{

	//create input nodes
	TArray<float> input;


	//add results to NeuralNetworkInput	

	/*inputs needs a more elegant solution built
	for every sensor we need a float returned with a value to apply to a function below.
	its clunky...
	*/
	input.Emplace(AISensors());
	input.Emplace(SideSensor());



	///Update fitness 


	//update brain with array of all inputs and get feedback
	MyBrain->Update(input);


	//return value for whatever function we're going to call
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("NN has %d inputs"), MyBrain->NeuralNetwork.Num()));
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("inputs has %d inputs"), input.Num()));
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("NNoutput has %d inputs"), NNoutput.Num()));
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("NNoutput has %d inputs"), input.Num()));
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("--------------------------------------input value is: %d "), input[0]));

		
		MoveForward(input[0]);
		MoveRight(input[1]);
		//Onfire
		//MoveLeft
		//MoveBackwards

		//jump

		//run away

		//
	







}

float AWorthyAICharacter::AISensors()
{
	//trace towards current movement direction
	//start trace variables
	FHitResult myHitResult;

	FVector TraceStart;
	FRotator ActorRotation;

	TraceStart = GetActorLocation();

	ActorRotation = GetActorRotation();


	FVector EndLocation = TraceStart + ActorRotation.Vector() * 1000;
	//FVector EndLocation = FMath::VRandCone(TraceStart, 50) + (EyeRotatoin.Vector() + 10000);
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	collisionParams.bTraceComplex = true;
	DrawDebugLine(GetWorld(), TraceStart, EndLocation, FColor::Green, false, 1.f, 0, 1.0f);

	//check hit
	if (GetWorld()->LineTraceSingleByChannel(myHitResult, TraceStart, EndLocation, ECollisionChannel::ECC_Visibility, collisionParams))
	{
		//debug line
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Hit"));
		MyBrain->IncrementFItness(4);

		return (GetActorLocation() - myHitResult.Actor->GetActorLocation()).Size();


	}
	else
	{
		MyBrain->DecrementFitness(1);
		return 0;
	}
}





float AWorthyAICharacter::SideSensor()
{
	//trace towards current movement direction
	//start trace variables
	FHitResult myHitResult;

	FVector TraceStart;
	FRotator ActorRotation;

	TraceStart = GetActorLocation();

	ActorRotation = GetActorRotation();


	FVector EndLocation = TraceStart + ActorRotation.Vector() * 1000;
	//FVector EndLocation = FMath::VRandCone(TraceStart, 50) + (EyeRotatoin.Vector() + 10000);
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	collisionParams.bTraceComplex = true;
	DrawDebugLine(GetWorld(), TraceStart, EndLocation, FColor::Green, false, 1.f, 0, 1.0f);

	//check hit
	if (GetWorld()->LineTraceSingleByChannel(myHitResult, TraceStart, EndLocation, ECollisionChannel::ECC_Visibility, collisionParams))
	{
		//debug line
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Hit"));
		MyBrain->IncrementFItness(4);

		return (GetActorLocation() - myHitResult.Actor->GetActorLocation()).Size();


	}
	else
	{
		MyBrain->DecrementFitness(2);
		return 0;
	}
}









