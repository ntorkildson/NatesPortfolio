// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WorthyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "WorthyWeapon.h"
#include "WorthyCharacter.h"
#include "Math/UnrealMathUtility.h"


void AWorthyProjectile::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    AWorthyWeapon *OwnerWeapon = Cast<AWorthyWeapon>(GetOwner());

    if (OwnerWeapon)
    {
        OwnerWeapon->ApplyWeaponConfig(MyWeapon);
    }

}

void AWorthyProjectile::RadiusAffect()
{

}

AWorthyProjectile::AWorthyProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AWorthyProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	MaxImpactRange = CreateDefaultSubobject<USphereComponent>(TEXT("MaxImpactRange"));
	MaxImpactRange->InitSphereRadius(128.0f);
	MaxImpactRange->BodyInstance.SetCollisionProfileName("Projectile");

	//MaxImpactRange->OnComponentHit.GetAllObjects();
	//MaxImpactRange->OnComponentHit.AddDynamic(this, &AWorthyProjectile::RadiusAffect);		// set up a notification for when this component hits something blocking
	MaxImpactRange->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	MaxImpactRange->CanCharacterStepUpOn = ECB_No;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;


}

void AWorthyProjectile::FindChainTargets()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Should Chain")));

	TArray<AActor*> OutOverlappingActors;
	MaxImpactRange->GetOverlappingActors(OutOverlappingActors);
	for (int32 i = 0; i >= OutOverlappingActors.Num(); i++)
	{

	}

	/*
	TArray<AWorthyCharacter*> Targets = Cast<AWorthyCharacter*>(MaxImpactRange->OnComponentHit.GetAllObjects());

	int32 myRandomNumber = FMath::FRandRange(0, Targets.Num());

	FVector MyLocation = GetActorLocation();
	AWorthyCharacter* temp = Targets[myRandomNumber -1];
	FVector targetLocation = temp->GetActorLocation();
	


	ProjectileMovement->Velocity = targetLocation;

	for (int32 i = 0; i >=MyWeapon.NumberOfChains; i++)
	{
				
	}

	/*
	for each number of chains
		-find nearest actor to chain to
		-set projectile vector to hit target
		-decrement chain counter
	*/
}


void AWorthyProjectile::PierceCounter()
{
	/*
	if projectile has pierce, which it does at this point
		-hit target but dont destroy self
	*/

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, (TEXT("Piercing")));

	if (CurrentPierceCount >= MyWeapon.NumberOfPierce)
	{
		Destroy();

	}
	CurrentPierceCount += 1;
}


void AWorthyProjectile::BounceCounter()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, (TEXT("Bouncing")));

}

void AWorthyProjectile::GeneateFork()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, (TEXT("Forked")));

}


void AWorthyProjectile::DealDamage(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

void AWorthyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (Role == ROLE_Authority)
	{
		FDamageEvent currentDamage;


		//deal damage to single target
		if (MyWeapon.NumberOfPierce < CurrentPierceCount)
		{
			PierceCounter();
			OtherActor->TakeDamage(MyWeapon.Damage, currentDamage, GetInstigatorController(), GetOwner());
		}
		else
		{
			CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
			//CollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);
			CollisionComp->UpdateCollisionProfile();
		}

		//order matters, can we make it editable in the editor?
		currentBounce += 1;
		if (currentBounce >= MyWeapon.NumberOfBounce)
		{
			Destroy();
		}

		//pierce first
		//fork
		//chain
		//bounce
		//explode last



		//check for on hit effects(burning, freeze,shock,poison,etc...
		//check for damage penetration(flat and %
		//check damage type(blunt,fire,lightning,etc,...)
		//apply damage

		else
		{
			FDamageEvent currentDamage;

			OtherActor->TakeDamage(MyWeapon.Damage, currentDamage, GetInstigatorController(), GetOwner());

			// Only add impulse and destroy projectile if we hit a physics
			if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

				Destroy();
			}
		}

	}
}


void AWorthyProjectile::BeginPlay()
{
    Super::BeginPlay();

	if (MyWeapon.NumberOfPierce > 1)
	{
		CollisionComp->BodyInstance.SetCollisionProfileName("Pierce");
		CollisionComp->UpdateCollisionProfile();
	}
}

