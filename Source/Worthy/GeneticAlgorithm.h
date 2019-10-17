// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorthyCharacter.h"
#include "GameFramework/Actor.h"
#include "GeneticAlgorithm.generated.h"






template<typename GA>
GA CrossOver(GA mom, GA dad)
{
	if (FMath::RandRange(0, 2) % 2 == 0)
	{
		return dad;
	}
	else
		return mom;

}

template<typename HA>
HA Mutation(HA &stat, float mutationRate, HA maxPertubation)
{
	float baseRoll;
	baseRoll = FMath::Rand() % 100;

	if (baseRoll <= mutationRate)
	{
		int32 testRoll = FMath::RandRange(0, 100);
			if (testRoll <= 50)
		{
			//Mutation increase
			stat += rand() % maxPertubation + 1;

			return stat;

		}
		else
		{
			//mutation decrease
			return stat -= rand() % maxPertubation - 1;
		}
	} 
	else
	{
		//no mutation
		return stat;
	}
}




//TODO: turn this into an Actor component
UCLASS()
class WORTHY_API AGeneticAlgorithm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneticAlgorithm();

	virtual float AssignFitness(); //survival, mate finding, reprodcution, protection, goals

	virtual void RouletteSelection();

	virtual void Adaptation();	//TODO: adaptions are leftover mutations or traits no longer in use, creatures can actively use these to increase fitnes

	virtual void Coevolution();//TODO: https://evolution.berkeley.edu/evolibrary/article/0_0_0/evo_34

	//TODO: template this, testing required
	UFUNCTION(BlueprintCallable, Category = "Genetic Algorithm")
	FGenotype BabyMaker(FGenotype& mom, FGenotype& dad); //returns a new child of whatever stats we have


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
