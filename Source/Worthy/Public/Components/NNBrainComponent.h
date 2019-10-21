// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NNBrainComponent.generated.h"


//a basic neuron, weights and inputs from the previous layer
USTRUCT(BlueprintType)
struct FNeuron
{
	GENERATED_BODY()

		UPROPERTY()
		int32 inputs;

	UPROPERTY()
	TArray<float> weights;

	FNeuron()
	{
		inputs = 1;
		for (int32 i = 0; i < inputs; i++)
		{
			weights.Emplace(FMath::RandRange(0.0f, 1.0f));
		}
	}
};

//stores the number of neurons in its layer
USTRUCT(BlueprintType)
struct FNeuronLayer
{
	GENERATED_BODY()
	
		UPROPERTY()
		int32 NumNeurons;

	UPROPERTY()
		TArray<FNeuron> VecNeurons;

	UPROPERTY()
		int32 NumInputsPerNeuron;

	FNeuronLayer()
	{

		//NumNeurons = 0;
		for (int32 i = 0; i<NumNeurons; i++)
		{
			VecNeurons.SetNum(NumInputsPerNeuron);
		}
	}

};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WORTHY_API UNNBrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNNBrainComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//building an actual NN that gets played with by a GA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int NumberOfInputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int NumberOfOutputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int NumberOfHiddenLayers;

	int NeuronsPerHiddenLayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<FNeuronLayer> NeuralNetwork;

	void CreateNetwork();

	int GetNumberOfWeights() const;


	void PutWeights(TArray<float> &weights);


	//calculates the outputs based on the inputs
	TArray<float> Update(TArray<float> &inputs);


	//TODO: change to RELU
	inline float Sigmoid(float activation, float response);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neural Network")
	float Bias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neural Network")
	float activationResponse;
};



