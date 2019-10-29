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

	UE_LOG(LogTemp, Log, TEXT("Number of Hidden layers: %d"), NumberOfHiddenLayers);

	//create layers of NN
	if (NumberOfHiddenLayers > 0)
	{

		UE_LOG(LogTemp, Log, TEXT("Number of Inputs: %d"), NumberOfInputs);
		UE_LOG(LogTemp, Log, TEXT("Number of Nuerons per hidden layer: %d"), NeuronsPerHiddenLayer);

		NeuralNetwork.Emplace(FNeuronLayer(NeuronsPerHiddenLayer, NumberOfInputs));
		

		UE_LOG(LogTemp, Log, TEXT("Number of Hidden Layers: %d"), NumberOfHiddenLayers);

		//hidden layers
		for (int32 i = 0; i < NumberOfHiddenLayers; i++)
		{	

			NeuralNetwork.Emplace(FNeuronLayer(NeuronsPerHiddenLayer, NumberOfHiddenLayers));


		}

		//output layer
		UE_LOG(LogTemp, Log, TEXT("------Number of OUtputs: %d"), NumberOfOutputs);

		NeuralNetwork.Emplace(FNeuronLayer(NumberOfOutputs, NeuronsPerHiddenLayer));

		UE_LOG(LogTemp, Log, TEXT("Size of Neural Network: %d"), NeuralNetwork.Num());
		
		UE_LOG(LogTemp, Log, TEXT("Num Neurons input layer %d"), NeuralNetwork[0].NumNeurons);
		UE_LOG(LogTemp, Log, TEXT("Num inputs per Neuron layer 0 %d"),NeuralNetwork[0].NumInputsPerNeuron);
		UE_LOG(LogTemp, Log, TEXT("Number of Neurons in Neural network 0: %d"), NeuralNetwork[0].VecNeurons.Num());
		

		UE_LOG(LogTemp, Log, TEXT("Number of weights per neuron in Neural network 0: %d"), NeuralNetwork[0].VecNeurons[0].weights.Num());
		UE_LOG(LogTemp, Log, TEXT("Number of inputs per neuron  in Neural network 0: %d"), NeuralNetwork[0].VecNeurons[0].inputs);


	

	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error Creating Neural Network"));

		NeuralNetwork.Emplace(FNeuronLayer(NumberOfOutputs, NumberOfInputs));
	}

	GetNumberOfWeights();
}



int UNNBrainComponent::GetNumberOfWeights() const
{
	
	int32 weights = 0;

	for (int32 i = 0; i < NumberOfHiddenLayers; i++)
	{

		for (int j = 0; j < NeuralNetwork[i].NumNeurons; j++)
		{
			for (int32 k = 0; k<NeuralNetwork[i].VecNeurons[j].inputs; k++)
			{
				weights++;
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("total number of weights: %d"), weights);



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
		UE_LOG(LogTemp, Error, TEXT("Number of Inputs is incorrect, please check UNNBrainComponent::UPdate()"));

 		return outputs;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Number of Hidden layers: %d"), NumberOfHiddenLayers);

	//for each hidden layer
	for (int32 i = 0; NumberOfHiddenLayers+1 > i; i++) // +1 is the bias weight
	{
		//UE_LOG(LogTemp, Error, TEXT("Show this message this many times NumHiddenLayers(): %d"), NumberOfHiddenLayers+1);
		if (i > 0)
		{
			//inputs = outputs;  //hey this breaks the whole thing!
		}



		//for each neuron
				//sum the inputs * weights, send total to signmoid function and return output			
		for (int32 j = 0; j < NeuralNetwork[i].NumNeurons; j++)
		{
			//UE_LOG(LogTemp, Error, TEXT("----------Show this message this many times  NumNeurons(): %d"), NeuralNetwork[i].NumNeurons);

			float netInput = 0;

			int32 NumInputs = NeuralNetwork[i].VecNeurons[j].inputs;

			//for each weights
			for (int32 k = 0;  NumInputs -1 >= k; k++)
			{
			//	UE_LOG(LogTemp, Error, TEXT("----------Show this message this many times(num inputs): %d"), NumInputs-1);
				//sum the weights * inputs
				netInput += NeuralNetwork[i].VecNeurons[j].weights[k] * inputs[cWeight];
				
				//UE_LOG(LogTemp, Warning, TEXT("----------------Size of Weights array %d"), NeuralNetwork[i].VecNeurons[j].weights.Num());
				//UE_LOG(LogTemp, Warning, TEXT("----------------Size of Weights array %f"), NeuralNetwork[i].VecNeurons[j].weights[k]);

				//UE_LOG(LogTemp, Warning, TEXT("-----------------k value is:  %d"), k);
				cWeight++;
			///	UE_LOG(LogTemp, Warning, TEXT("tempweight is: %d"), cWeight);
				//UE_LOG(LogTemp, Warning, TEXT("netInput is: %d"), netInput);
				//UE_LOG(LogTemp, Warning, TEXT("input count is: %d"), inputs.Num());

			}


			//add in the bias
			netInput += NeuralNetwork[i].VecNeurons[j].weights[NumInputs - 1] * Bias;


			//store ouputs from each layer as they are generated, combined with sigmoid function
			outputs.Emplace(Sigmoid(netInput, activationResponse));

			//reset
			cWeight = 0;
		}

	}

	//UE_LOG(LogTemp, Warning, TEXT("----------------------LOOP RESET-----------------------------"));


	return outputs;

}


TArray<float> UNNBrainComponent::GetWeights() const
{
	TArray<float> weights;
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("NN GetWeights()"));

	//for each layer
	for (int i = 0; i < NumberOfHiddenLayers; i++)
	{

		//for each neuron
		for (int j = 0; j < NeuralNetwork[i].NumNeurons; j++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Silver, FString::Printf(TEXT("----Size of Neural Network is:  %d "), NeuralNetwork.Num()));
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Silver, FString::Printf(TEXT("--------NN VecNeurons are:  %d"), NeuralNetwork[i].VecNeurons.Num()));

			//for each weight
			for (int k = 0; k < NeuralNetwork[i].VecNeurons[j].inputs+1; k++)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, TEXT("------Get Weightd"));

				weights.Emplace(NeuralNetwork[i].VecNeurons[j].weights[k]);
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("-------NN weights are %f "), NeuralNetwork[i].VecNeurons[j].weights[k]));

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

