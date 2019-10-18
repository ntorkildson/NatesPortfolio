// Fill out your copyright notice in the Description page of Project Settings.


#include "WorthyWeapon.h"
#include "WorthyCharacter.h"
#include "WorthyProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void AWorthyWeapon::ApplyWeaponConfig(FWeaponData &weaponInfo)
{
    weaponInfo = WeaponStats;
}

// Sets default values
AWorthyWeapon::AWorthyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    SetReplicates(true);



    FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    FP_MuzzleLocation->SetupAttachment(ItemMesh);
    FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));


}

// Called when the game starts or when spawned
void AWorthyWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWorthyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorthyWeapon::PlayEffects()
{
	//TODO: send Firing animation/ attacking animation to owning pawn

    // try and play the sound if specified
    if (FireSound != NULL)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    // try and play a firing animation if specified
    if (FireAnimation != NULL)
    {
        // Get the animation object for the arms mesh
        UAnimInstance *AnimInstance = ItemMesh->GetAnimInstance();
        if (AnimInstance != NULL)
        {
            AnimInstance->Montage_Play(FireAnimation, 1.f);
        }
    }
}

void AWorthyWeapon::OnFire()
{
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}
	if (!bIsFIring)
	{
		bIsFIring = true;

		WeaponTimer();
	}
}

void AWorthyWeapon::StopFire()
{
    //start a timer to allow firing again -disallows left click spamming
    if (Role < ROLE_Authority)
    {
        ServerStopFire();
    }
    GetWorld()->GetTimerManager().SetTimer(SafeFireHandle, this, &AWorthyWeapon::ResetWeapon,
                                           (TimeBetweenShots) * NumberOfBurst, true);

}

bool AWorthyWeapon::ServerStopFire_Validate()
{
    return true;

}

void AWorthyWeapon::ServerStopFire_Implementation()
{
    StopFire();
}

void AWorthyWeapon::ResetWeapon()
{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, (TEXT("Reset")));

    //reset all the firing variables and allow shooting to start again
    GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(SafeFireHandle);
    bIsFIring = false;

}

void AWorthyWeapon::useAmmo()
{
    if (currentAmmo != 0)
        currentAmmo -= 1;
    else if (currentClips != 0)
        {
            currentAmmo = maxAmmo;
            currentClips -= 1;

        }
    
	else
	{
	bCanFire = false;

	}
}

void AWorthyWeapon::OnEquip()
{

	
}



void AWorthyWeapon::FireProjectile()
{
	if (Role == ROLE_Authority)
	{
		// try and fire a projectile
		if (ProjectileClass != NULL)
		{
			UWorld *const World = GetWorld();
			if (World != NULL)
			{

				const FRotator SpawnRotation = GetOwner()->GetInstigatorController()->GetControlRotation();

				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation =
					((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) +
					SpawnRotation.RotateVector(GunOffset);

				FTransform SpawnTransform(SpawnRotation, SpawnLocation);
				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				// spawn the projectile at the muzzle, buckshot firing
				for (int32 i = 0; i < numberOfProjectiles; i++)
				{
					LastFireTime = GetWorld()->GetTimeSeconds();

					/***Deferred spawn WIP*/
					AWorthyProjectile *Projectile = Cast<AWorthyProjectile>(
						UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform,
							ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
							this));
					if (Projectile)
					{

						Projectile->Instigator = Instigator;
						Projectile->SetOwner(this);

						UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);

					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Projectile DOesnt exist"));
						World->SpawnActor<AWorthyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation,
							ActorSpawnParams);

					}


				}

			}
		}
	}
	useAmmo();
	PlayEffects();

}

void AWorthyWeapon::FireTrace()
{
	if (Role == ROLE_Authority && bCanFire)
	{
		AActor* MyOwner = GetOwner();

		if (MyOwner)
		{
			//start trace variables
			FHitResult myHitResult;
			//FVector EyeLocation = MyOwner->GetActorLocation();
			//const FRotator EyeRotation = GetOwner()->GetInstigatorController()->GetControlRotation();

			FVector EyeLocation;
			FRotator EyeRotation;

			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);  
			
			EyeRotation = GetOwner()->GetActorRotation();


			FVector EndLocation = EyeLocation + EyeRotation.Vector() * 1000;
			//FVector EndLocation = FMath::VRandCone(EyeLocation, 50) + (EyeRotatoin.Vector() + 10000);
			FCollisionQueryParams collisionParams;
			collisionParams.AddIgnoredActor(MyOwner);
			collisionParams.AddIgnoredActor(this);
			collisionParams.bTraceComplex = true;
			DrawDebugLine(GetWorld(), EyeLocation, EndLocation, FColor::Red, false, 1.f, 0, 1.0f);

			//check hit
			if (GetWorld()->LineTraceSingleByChannel(myHitResult, EyeLocation, EndLocation, ECollisionChannel::ECC_Visibility, collisionParams))
			{
				//debug line
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("firing"));

				//deal damage/physics/effects/etc.

			}

			useAmmo();
			PlayEffects();

			//
		}
	}
}



void AWorthyWeapon::ServerFire_Implementation()
{
	OnFire();
}

bool AWorthyWeapon::ServerFire_Validate()
{
    return true;
}

void AWorthyWeapon::WeaponTimer()
{

    if (bIsFIring == true)
    {

        //GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this, &AWorthyWeapon::FireProjectile,TimeBetweenShots,true);
		GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this, &AWorthyWeapon::FireTrace, TimeBetweenShots, true);

    }


}



