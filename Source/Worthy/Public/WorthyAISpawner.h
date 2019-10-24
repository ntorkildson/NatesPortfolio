// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorthyAISpawner.generated.h"


class AWorthyAICharacter;

class AWorthyWeapon;

UCLASS()
class WORTHY_API AWorthyAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorthyAISpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "AI SPawning")
		void SpawnAIClass();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Spawning")
		TSubclassOf<AWorthyAICharacter> AIClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Spawning")
		int32 NumberOfAI;


	class AGeneticAlgorithm* GeneticALgorithm;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Spawning")
	TArray<AWorthyAICharacter*> FullPopulation;

	void RunSimulation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Spawning")
		int32 LegnthOfSimulation;

	int32 GenerationCounter;

};

