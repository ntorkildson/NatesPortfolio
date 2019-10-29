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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

		UPROPERTY(VisibleDefaultsOnly, Category = "AI")
		class USphereComponent * PerceptionRange;

		UFUNCTION(BlueprintCallable, Category = "AI")
			void UpdateBrain();


		//MyBrain
		UPROPERTY(VisibleDefaultsOnly, Category = "AI")
			class UNNBrainComponent* MyBrain;



		UPROPERTY(EditAnywhere, Category = "AI")
			float SenseRadius;

		EAIChoice ActionEvent;


	

		float forwardSensor();
		float leftSensor();
		float rightSensor();


		int32 GenerationCounter=0;

};

