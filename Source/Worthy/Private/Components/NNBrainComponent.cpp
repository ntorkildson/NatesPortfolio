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
	CreateNetwork();
}


// Called every frame
void UNNBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNNBrainComponent::CreateNetwork()
{


	//create layers of NN
	if (NumberOfHiddenLayers > 0)
	{

		NeuralNetwork.Emplace(FNeuronLayer(NeuronsPerHiddenLayer, NumberOfInputs));
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Neurons per hidden layer: %d "), NeuronsPerHiddenLayer));
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("----Number of Inputs: %d "), NumberOfInputs));


		//hidden layers
		for (int32 i = 0; i < NumberOfHiddenLayers - 1; i++)
		{	

			NeuralNetwork.Emplace(FNeuronLayer(NeuronsPerHiddenLayer, NumberOfHiddenLayers));
		//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, FString::Printf(TEXT("-------Neurons per hidden layer: %d "), NeuronsPerHiddenLayer));
		//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, FString::Printf(TEXT("-------Number of Inputs: %d "), NeuronsPerHiddenLayer));

		}

		//output layer
	
		NeuralNetwork.Emplace(FNeuronLayer(NumberOfOutputs, NeuronsPerHiddenLayer));

	//	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("-----------Number of outputs: %d "), NumberOfOutputs));
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("-----------Neurons per hidden layer: %d "), NeuronsPerHiddenLayer));
	}

	else
	{
		NeuralNetwork.Emplace(FNeuronLayer(NumberOfOutputs, NumberOfInputs));
	}

}



int UNNBrainComponent::GetNumberOfWeights() const
{
	int32 weights = 0;

	for (int32 i = 0; i < NumberOfHiddenLayers; i++)
	{

		for (int j = 0; j < NeuralNetwork[i].NumNeurons; i++)
		{
			for (int32 k = 0; k<NeuralNetwork[i].VecNeurons[j].inputs; k++)
			{
				weights++;
			}
		}
	}
	return weights;
}

void UNNBrainComponent::PutWeights(TArray<float> &weights)
{
	
	int32 tempWeight = 0;
	for (int32 i = 0; i < NumberOfHiddenLayers; i++)
	{
		for(int32 j = 0; j<NeuralNetwork[i].NumNeurons; j++)
			//each weight
			for (int32 k = 0; k < NeuralNetwork[i].VecNeurons[j].inputs; k++)
			{
				NeuralNetwork[i].VecNeurons[j].weights[k] = weights[tempWeight++];
			}
	}
	
}


void UNNBrainComponent::IncrementFItness(int32 amount)
{
	Fitness+=amount;
}

void UNNBrainComponent::DecrementFitness(int32 amount)
{
	Fitness-= amount;
}

TArray<float> UNNBrainComponent::Update(TArray<float> &inputs)
{
	TArray<float> outputs;

	int32 cWeight = 0;

	
	//check the inputs are correct
 	if (inputs.Num() != NumberOfInputs)
 	{
 		return outputs;
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("something is horribly wrong "));
 	}
	
	for (int32 i = 0; i < NumberOfHiddenLayers + 1; i++) // +1 is the bias weight
	{
		//TODO: double check this, I have a feeling its wrong
		//for each neuron, sum the inputs and weights and * by sigmoid/relu to get result
		for (int32 j = 0; j < NeuralNetwork[i].NumNeurons; j++)
		{
			float netInput = 0;

			int NumInputs = NeuralNetwork[i].VecNeurons[j].inputs;

			for (int k = 0; k < NumberOfInputs - 1; ++k)
			{
				//sumn weights * inputs
				netInput += NeuralNetwork[i].VecNeurons[j].weights[k] * inputs[cWeight++];

			}

			//add bias
			netInput += NeuralNetwork[i].VecNeurons[j].weights[NumInputs + 1] * Bias;
			//store outputs for each layer as we gnerate
			outputs.Emplace(Sigmoid(netInput, activationResponse));

		}
		
	}
	
	return outputs;
}


	





TArray<float> UNNBrainComponent::GetWeights() const
{
	TArray<float> weights;

	//for each layer
	for (int i = 0; i < NumberOfHiddenLayers + 1; ++i)
	{

		//for each neuron
		for (int j = 0; j < NeuralNetwork[i].NumNeurons; ++j)
		{
			//for each weight
			for (int k = 0; k < NeuralNetwork[i].VecNeurons[j].inputs; ++k)
			{
				weights.Emplace(NeuralNetwork[i].VecNeurons[j].weights[k]);
			}
		}
	}

	return weights;
}

float UNNBrainComponent::Sigmoid(float activation, float response)
{
	return (1 / (1 + exp(-activation / response)));

}

void UNNBrainComponent::RunSimulation()
{
	//how often do we cycle the system

	//for each AI, update NN with inputs

	//if 'events' happen adjust fitness accordingly

	//update chromosones fitness score

	//generation complete

		//run GA and update brains

			//average fitness
			//best fitness
			//increment generation counter
			//reset timer
			//insert new brain into AI
			//reset temp brain
}

