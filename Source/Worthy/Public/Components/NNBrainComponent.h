// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NNBrainComponent.generated.h"


//a basic neuron, weights and inputs from the previous layer
USTRUCT(immutable, noexport, BlueprintType)
struct FNeuron
{

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IntVector2D")
		int32 inputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IntVector2D")
		TArray<float> weights;

	FNeuron();
	
	FNeuron(int32 numInputs) : inputs(numInputs)
	{
		inputs = numInputs;
		for (int32 i = 0; i < numInputs + 1; i++)
		{
			float test = (FMath::RandRange(0.0f, 1.0f));
			UE_LOG(LogTemp, Error, TEXT("--------------------weights are  : %f"), test);

			weights.Emplace(test);
		}
	}


};



//stores the number of neurons in its layer
USTRUCT(noexport, BlueprintType)
struct FNeuronLayer
{

		UPROPERTY()
		int32 NumNeurons;

	UPROPERTY()
		TArray<FNeuron> VecNeurons;

	UPROPERTY()
		int32 NumInputsPerNeuron;

	FNeuronLayer();
	FNeuronLayer(int32 NumNeurons_, int32 NumInputsPerNeuron_) : NumNeurons(NumNeurons_), NumInputsPerNeuron(NumInputsPerNeuron_)
	{
		NumNeurons = NumNeurons_;
		NumInputsPerNeuron = NumInputsPerNeuron_;
		for (int32 i = 0; i < NumNeurons_; i++)
		{
			UE_LOG(LogTemp, Log, TEXT("NumberNeurons_ : %d"), NumNeurons_);
			UE_LOG(LogTemp, Log, TEXT("NumInputsPerNeuron_ %d"), NumInputsPerNeuron_);

			VecNeurons.Emplace(FNeuron(NumInputsPerNeuron_));
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neural Network")
	int32 NumberOfInputs=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neural Network")
	int32 NumberOfOutputs=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neural Network")
	int32 NumberOfHiddenLayers = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neural Network")
	int32 NeuronsPerHiddenLayer=15;

	TArray<FNeuronLayer> NeuralNetwork;

	void CreateNetwork();

	int32 GetNumberOfWeights() const;

	int32 Fitness;

	void PutWeights(TArray<float> &weights);

	void IncrementFItness(int32 amount);

	void DecrementFitness(int32 amount);

	//calculates the outputs based on the inputs
	TArray<float> Update(TArray<float> &inputs);


	TArray<float>  GetWeights() const;


	//TODO: change to RELU
	inline float Sigmoid(float activation, float response);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neural Network")
	float Bias = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neural Network")
	float activationResponse = -1.0f;

	void RunSimulation();

	int32 SimulationTime;


};



