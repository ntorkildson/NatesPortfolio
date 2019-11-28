// Fill out your copyright notice in the Description page of Project Settings.


#include "WorthyBuilding.h"


AWorthyBuilding::AWorthyBuilding()
{
	//MyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

}

void AWorthyBuilding::UseItem()
{
	//on use 
}

void AWorthyBuilding::DropItem()
{
	//should spawn the item on the appropriate grid
		///grid is 160 units currently

	//use tree structure? to do physics calculations


	//place structure down, verify all local spawning bays can reach the base portal thingy
}
