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
	input.Emplace(forwardSensor());
	input.Emplace(leftSensor());
	input.Emplace(rightSensor());



	///Update fitness 


	//update brain with array of all inputs and get feedback
	UE_LOG(LogTemp, Log, TEXT("Number of inputs from AICharacter: %d"), input.Num());

	MyBrain->Update(input);


	//return value for whatever function we're going to call


	UE_LOG(LogTemp, Log, TEXT("forward value is: %f"), input[0]);
	UE_LOG(LogTemp, Log, TEXT("right value is: %f"), input[1]);

		MoveForward(input[0]);
		MoveRight(input[1]); //SetActorRotation((input *2 -1)+ ActorRotation.Z )
		//Onfire
		//MoveLeft
		//MoveBackwards

		//jump

		//run away

		//


}



float AWorthyAICharacter::forwardSensor()
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
		MyBrain->IncrementFItness(1);
		UE_LOG(LogTemp, Log, TEXT("current fitness is: %d"), MyBrain->Fitness);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("TESTING"));

		return (GetActorLocation() - myHitResult.Actor->GetActorLocation()).Size() *-1;


	}
	else
	{
		MyBrain->DecrementFitness(10);
		UE_LOG(LogTemp, Log, TEXT("current fitness is: %d"), MyBrain->Fitness);

		return 1;
	}
}

float AWorthyAICharacter::leftSensor()
{
	//trace towards current movement direction
	//start trace variables
	FHitResult myHitResult;

	FVector TraceStart;
	FRotator ActorRotation;

	TraceStart = GetActorLocation();

	ActorRotation = GetActorRotation();

	FVector EndLocation = TraceStart + EndLocation.RotateAngleAxis(45, GetActorForwardVector() * 5);


	//FVector EndLocation = FMath::VRandCone(TraceStart, 50) + (EyeRotatoin.Vector() + 10000);
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	collisionParams.bTraceComplex = true;
	DrawDebugLine(GetWorld(), TraceStart, EndLocation, FColor::Red, false, 1.f, 0, 1.0f);

	//check hit
	if (GetWorld()->LineTraceSingleByChannel(myHitResult, TraceStart, EndLocation, ECollisionChannel::ECC_Visibility, collisionParams))
	{
		//debug line
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit"));
		MyBrain->IncrementFItness(1);
		UE_LOG(LogTemp, Log, TEXT("current fitness is: %d"), MyBrain->Fitness);

		return 1;


	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("current fitness is: %d"), MyBrain->Fitness);

		MyBrain->DecrementFitness(20);
		return -1;
	}
}

float AWorthyAICharacter::rightSensor()
{
	//trace towards current movement direction
		//start trace variables
	FHitResult myHitResult;

	FVector TraceStart;
	FRotator ActorRotation;

	TraceStart = GetActorLocation();

	ActorRotation = GetActorRotation();

	FVector EndLocation = TraceStart + EndLocation.RotateAngleAxis(-45, GetActorForwardVector() * 5);


	//FVector EndLocation = FMath::VRandCone(TraceStart, 50) + (EyeRotatoin.Vector() + 10000);
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	collisionParams.bTraceComplex = true;
	DrawDebugLine(GetWorld(), TraceStart, EndLocation, FColor::Red, false, 1.f, 0, 1.0f);

	//check hit
	if (GetWorld()->LineTraceSingleByChannel(myHitResult, TraceStart, EndLocation, ECollisionChannel::ECC_Visibility, collisionParams))
	{
		//debug line
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit"));
		MyBrain->IncrementFItness(1);
		UE_LOG(LogTemp, Log, TEXT("current fitness is: %d"), MyBrain->Fitness);

		return 1;


	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("current fitness is: %d"), MyBrain->Fitness);

		MyBrain->DecrementFitness(20);
		return -1;
	}
}






