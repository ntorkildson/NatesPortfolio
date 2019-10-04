// Fill out your copyright notice in the Description page of Project Settings.


#include "WorthyWeapon.h"

// Sets default values
AWorthyWeapon::AWorthyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorthyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorthyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

