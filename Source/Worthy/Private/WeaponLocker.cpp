// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponLocker.h"
#include "WorthyItem.h"
#include "WorthyCharacter.h"

AWeaponLocker::AWeaponLocker()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LockerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = LockerMesh;
	LockerMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	LockerMesh->SetSimulatePhysics(true);
	SetReplicates(true);
	bReplicateMovement = true;
	LockerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AWeaponLocker::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//auto *Owner = Cast<AWorthyCharacter>(GetOwner());

	AWorthyItem *Owner = Cast<AWorthyItem>(GetOwner());
	if (Owner)
	{
		UE_LOG(LogTemp, Log, TEXT("We have an owner!"));

		Owner->InitializeDroppedItem();
		//LockerMesh = Owner->ItemMesh;

	}

}


// Sets default values


// Called when the game starts or when spawned
void AWeaponLocker::BeginPlay()
{
	Super::BeginPlay();

	//incase someone forgets to make a locker with information in it
	if (ItemsInContainer.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawn some default values"));
		FItemInformation woops;
		woops.qty = 1;
		TSubclassOf<AWorthyItem> temp;
		woops.ItemInventory = temp;
		ItemsInContainer.Emplace(woops);
	}
}

// Called every frame
void AWeaponLocker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AWeaponLocker::InteractWith(AWorthyCharacter* InstigatorPawn)
{
	//TODO: Move from blueprints here
	AWorthyCharacter* MyPawn = Cast<AWorthyCharacter>(InstigatorPawn);
	if (MyPawn)
	{
		UE_LOG(LogTemp, Log, TEXT("interaction is working at least"));

		
		if (MyPawn->CurrentWeapon)
		{

			UE_LOG(LogTemp, Log, TEXT("add to inventory"));

		
			//get the number of items in the container
			for (int32 i = 0; i > ItemsInContainer.Num(); i++)
			{
				//get the qty of items to spawn
				for (int32 j = 0; j > ItemsInContainer[i].qty; j++)
				{

					//add them to inventory
					MyPawn->PlayerInventory.Emplace(ItemsInContainer[j]);
				}
			}
		}
		
		else
		{
			if (GetWorld())
			{
				UE_LOG(LogTemp, Log, TEXT("doesnt have weapon, should equip"));
				
				FName Socket = TEXT("GripPoint");
				TSubclassOf<AWorthyItem> test = ItemsInContainer[0].ItemInventory;
				MyPawn->EquipItem(Socket, test);

			}
	

		}
		Destroy();

	}
}

