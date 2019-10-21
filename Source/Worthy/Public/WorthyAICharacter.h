// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorthyCharacter.h"
#include "WorthyAICharacter.generated.h"



UENUM(BlueprintType)
enum class EAIChoice : uint8
{
	WalkForward,
	WalkBackward,
	Fire,

};

/**
 * 
 */
UCLASS()
class WORTHY_API AWorthyAICharacter : public AWorthyCharacter
{
	GENERATED_BODY()
	

	AWorthyAICharacter();

	UFUNCTION(BlueprintCallable, Category="AI")
	void Update(TArray<FVector> readings);

	//MyBrain
	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UNNBrainComponent* MyBrain;

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
		class USphereComponent* SensorRadius;

	UPROPERTY(EditAnywhere, Category = "AI")
		float SenseRadius;



	EAIChoice ActionEvent;



	class GeneticAlgorithm* MyMutation;

	void MutateMe();



};
