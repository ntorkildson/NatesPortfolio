// Fill out your copyright notice in the Description page of Project Settings.


#include "WorthyItem.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponLocker.h"

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


}

// Called every frame
void AWorthyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorthyItem::UseItem()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("USE ME BABY"));
}


void AWorthyItem::DropItem()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector CamLoc;
		FRotator CamRot;


		MyOwner->GetInstigatorController()->GetPlayerViewPoint(CamLoc, CamRot);
		FVector SpawnLocation;
		FRotator SpawnRotation = CamRot;
		FTransform SpawnTransform(SpawnRotation, SpawnLocation);

		const FVector Direction = CamRot.Vector();
		const FVector TraceStart = GetActorLocation();
		const FVector TraceEnd = TraceStart + Direction * 150;

		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = false;
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.AddIgnoredActor(this);

		FHitResult Hit;
		if (GetWorld())
		{
			GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldDynamic, TraceParams);

			/* Find farthest valid spawn location */
			if (Hit.bBlockingHit)
			{
				/* Slightly move away from impacted object */
				SpawnLocation = Hit.ImpactPoint + (Hit.ImpactNormal * 2);
			}
			else
			{
				SpawnLocation = TraceEnd;
			}

			/* Spawn the "dropped" weapon 
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AWeaponLocker* NewWeaponPickup = GetWorld()->SpawnActor<AWeaponLocker>(WeaponLockerClass, SpawnLocation, FRotator::ZeroRotator, SpawnInfo);
				*/

			

			AWeaponLocker *NewWeaponPickup = Cast<AWeaponLocker>(
				UGameplayStatics::BeginDeferredActorSpawnFromClass(this, WeaponLockerClass, SpawnTransform,
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
					this));

			if (NewWeaponPickup)
			{

				NewWeaponPickup->Instigator = Instigator;
				NewWeaponPickup->SetOwner(this);
				
				UGameplayStatics::FinishSpawningActor(NewWeaponPickup, SpawnTransform);
				Destroy();

			}
			else
			{ 
				UE_LOG(LogTemp, Warning, TEXT("Weapon Locker Spawning failed"));

			}
			
		}

	}
}


void AWorthyItem::InitializeDroppedItem()
{
	//pass by reference all of this items stats to the new item. 
	//mesh to use 
	//ItemStats
	//etc..

}

