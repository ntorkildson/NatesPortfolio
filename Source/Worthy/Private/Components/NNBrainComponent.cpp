 // Fill out your copyright notice in the Description page of Project Settings.


#include "NNBrainComponent.h"

// Sets default values for this component's properties
UNNBrainComponent::UNNBrainComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNNBrainComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNNBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNNBrainComponent::CreateNetwork()
{

}

int UNNBrainComponent::GetNumberOfWeights() const
{
	return 1;

}

void UNNBrainComponent::PutWeights(TArray<float> &weights)
{

}

TArray<float> UNNBrainComponent::Update(TArray<float> &inputs)
{

	TArray<float> outputs;

	int32 cWeight = 0;

	//check the inputs are correct
	if (inputs.Num() == NumberOfInputs)
	{

		for (int32 i = 0; i < NumberOfHiddenLayers + 1; i++) // +1 is the bias weight
		{
			if (i > 0)
			{
				inputs = outputs;
			}
			outputs.clear();

		}


		//for each neuron, sum the inputs and weights and * by sigmoid/relu to get result
		for (int32 k = 0; k < NeuralNetwork[i].NumNeurons; k++)
		{
			float netInput = 0;

			int NumInputs = NeuralNetwork[i].VecNeurons[k].inputs;

			for (int l = 0; l < NumberOfInputs - 1; ++l)
			{
				//sumn weights * inputs
				netInput += NeuralNetwork[i].VecNeurons[k].weights[NumInputs - 1] * Bias;

				outputs.Emplace(Sigmoid(netInput, activationResponse));
			}
		}


	}
	else
	{
		//returns and empty array
		return outputs;
	}
}

float UNNBrainComponent::Sigmoid(float activation, float response)
{
	return 1.0f;

}

