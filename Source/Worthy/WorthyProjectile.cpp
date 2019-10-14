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

void AWorthyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    if (Role == ROLE_Authority)
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



void AWorthyProjectile::BeginPlay()
{
    Super::BeginPlay();

}

