// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorthyItem.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    scrap    UMETA(DisplayName = "scrap"),
    normal    UMETA(DisplayName = "normal"),
    magic    UMETA(DisplayName = "magic"),
    rare    UMETA(DisplayName = "rare"),
    unique    UMETA(DisplayName = "unique"),
    legendary	UMETA(DisplayName = "legendary"),

};


USTRUCT(BlueprintType)
struct FItemStats
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, Category="ItemModifier")
    int32 DexModifier;

    UPROPERTY(EditDefaultsOnly, Category="ItemModifier")
    int32 StrModifier;

    UPROPERTY(EditDefaultsOnly, Category="ItemModifier")
    int32 IntModifier;

    FItemStats()
    {
        DexModifier  = 0;
        StrModifier = 0;
        IntModifier = 0;

    }

};

UCLASS()
class WORTHY_API AWorthyItem : public AActor
{
	GENERATED_BODY()
	
public:

UPROPERTY(EditDefaultsOnly, Category = "ItemStats")
USkeletalMeshComponent* ItemMesh;


UPROPERTY(EditDefaultsOnly, Category = "ItemStats")
FName ItemSocketName;
	// Sets default values for this actor's properties
	AWorthyItem();

UPROPERTY(VisibleDefaultsOnly, Category = "ItemStats")
int32 someRandomVarialbe;

UPROPERTY(EditDefaultsOnly, Category="ItemStats")
FItemStats ItemStats;

UPROPERTY(EditDefaultsOnly, Category ="ItemStats")
EItemRarity ItemRarity;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName AttachPoint;


	void UpdateStats();

    void AttachToSocket();


};
