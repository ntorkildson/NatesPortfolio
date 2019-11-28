// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WorthyItem.generated.h"



USTRUCT(BlueprintType)
struct FItemStats
{
	//GENERATED_USTRUCT_BODY()
		GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, Category = "ItemStats|ItemModifier")
		int32 DexModifier;

	UPROPERTY(EditDefaultsOnly, Category = "ItemStats|ItemModifier")
		int32 StrModifier;

	UPROPERTY(EditDefaultsOnly, Category = "ItemStats|ItemModifier")
		int32 IntModifier;

	UPROPERTY(EditDefaultsOnly, Category = "ItemStats")
		FName ItemSocketName;

	FItemStats()
	{
		DexModifier = 0;
		StrModifier = 0;
		IntModifier = 0;
		ItemSocketName = TEXT("WeaponGrip");
	}

};


UCLASS()
class WORTHY_API AWorthyItem : public AActor
{
	GENERATED_BODY()




public:
	// Sets default values for this actor's properties
	AWorthyItem();

UPROPERTY(EditDefaultsOnly, Category = "ItemStats")
USkeletalMeshComponent* ItemMesh;

//TODO: See if we can make this more automated.
UPROPERTY(EditDefaultsOnly, Category = "ItemStats")
TSubclassOf<class AWeaponLocker> WeaponLockerClass;

UPROPERTY(EditDefaultsOnly, Category="ItemStats")
FItemStats ItemStats;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName AttachPoint;


	//when Item is hit by a trace, despawn and add to inventory
	virtual void UseItem();

	void UnEquipItem(); //Unequips the item and places it in the inventory

	virtual void DropItem(); // takes item in inventory, spawns it and drops it on the ground.

	void InitializeDroppedItem();



};
