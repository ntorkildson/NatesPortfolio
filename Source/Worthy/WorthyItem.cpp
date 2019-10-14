// Fill out your copyright notice in the Description page of Project Settings.


#include "WorthyItem.h"


// Sets default values
AWorthyItem::AWorthyItem()
{
// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SetReplicates(true);

    // Create a gun mesh component
    ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = ItemMesh;
    // WeaponMesh->SetupAttachment(Mesh1P, TEXT("GripPoint"));
    //WeaponMesh->SetupAttachment(RootComponent);



}

// Called when the game starts or when spawned
void AWorthyItem::BeginPlay()
{
	Super::BeginPlay();

	// attach item to socket
	    //should be rep_notify
	//update character stats with weapon modifications

}

// Called every frame
void AWorthyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

