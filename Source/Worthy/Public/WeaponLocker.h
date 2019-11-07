// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponLocker.generated.h"

class AWorthyItem;

class USkeletalMeshComponent;



USTRUCT(BlueprintType)
struct FItemInformation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<AWorthyItem> ItemInventory;

	UPROPERTY(EditAnywhere, Category = "Item")
	int32 qty;

	FItemInformation()
	{
		ItemIcon = nullptr;
		ItemInventory = nullptr;
		qty = 1;
	}

};


UCLASS()
class WORTHY_API AWeaponLocker : public AActor
{
	GENERATED_BODY()

		virtual void PostInitializeComponents() override;

public:	
	// Sets default values for this actor's properties
	AWeaponLocker();

	UPROPERTY(EditDefaultsOnly, Category = "Item")
		UStaticMeshComponent* LockerMesh;

UPROPERTY(EditAnywhere, Category = "Item")
TArray<FItemInformation> ItemsInContainer;


//has a base setup, customization can be done in the editor
UFUNCTION(BlueprintCallable, Category = "Item")
void InteractWith(AWorthyCharacter* InstigatorPawn);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

