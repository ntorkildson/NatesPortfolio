// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorthyWeapon.generated.h"

UCLASS()
class WORTHY_API AWorthyWeapon : public AActor
{
	GENERATED_BODY()
/** Gun mesh: 1st person view (seen only by self) */
UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
class USkeletalMeshComponent* FP_Gun;
public:	
	// Sets default values for this actor's properties
	AWorthyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
