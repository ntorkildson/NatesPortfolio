// Fill out your copyright notice in the Description page of Project Settings.


#include "WorthyWeapon.h"
#include "WorthyCharacter.h"
#include "WorthyProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"


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

    // Create a gun mesh component
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;
    // WeaponMesh->SetupAttachment(Mesh1P, TEXT("GripPoint"));
    //WeaponMesh->SetupAttachment(RootComponent);

    FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    FP_MuzzleLocation->SetupAttachment(WeaponMesh);
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
    // try and play the sound if specified
    if (FireSound != NULL)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    // try and play a firing animation if specified
    if (FireAnimation != NULL)
    {
        // Get the animation object for the arms mesh
        UAnimInstance *AnimInstance = WeaponMesh->GetAnimInstance();
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
                                           TimeBetweenShots * NumberOfBurst, true);

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
    else
    {
        if (currentClips != 0)
        {
            currentAmmo = maxAmmo;
            currentClips -= 1;

        }
    }
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

    //while bisFIring == true && bhasammo == true
    if (bIsFIring == true)
    {

        GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this, &AWorthyWeapon::FireProjectile,
                                               TimeBetweenShots,
                                               true);
    }


}

