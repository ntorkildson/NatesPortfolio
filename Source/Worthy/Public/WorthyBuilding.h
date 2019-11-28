// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorthyItem.h"
#include "WorthyBuilding.generated.h"

class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class WORTHY_API AWorthyBuilding : public AWorthyItem
{
	GENERATED_BODY()

public:
		AWorthyBuilding();

		//using item is overriden
		virtual void UseItem() override;

		// dropping is overridden to spawn properly
	virtual void DropItem() override;

		//buildings can be walls/floors/traps/turrets/etc..
	//UStaticMeshComponent MyStaticMesh;
		//use item stats for life and all that.

		//

};
