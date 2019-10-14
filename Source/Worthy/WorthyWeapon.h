// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorthyItem.h"
#include "WorthyWeapon.generated.h"


class USkeletalMeshComponent;


UENUM(BlueprintType)  //TODO: make this useful
enum class EProjectileMods : uint8
{
    Explode    UMETA(DisplayName = "Explode"),
    Pierce    UMETA(DisplayName = "Pierce"),
    Fork    UMETA(DisplayName = "Fork"),
    Bounce    UMETA(DisplayName = "Bounce"),
    Chain    UMETA(DisplayName = "Chain"),
	Homing	UMETA(DisplayName = "Homing"),

};




USTRUCT(BlueprintType) //TODO: make this useful
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStats")
    int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStats")
	int32 ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStats")
	int32 NumberOfPierce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStats")
	int32 NumberOfFork;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStats")
	int32 NumberOfBounce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStats")
	int32 NumberOfChains;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponStats")
	AActor* ActorToFollow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStats")
    TArray<EProjectileMods> AllMods;




    FWeaponData()
    {
        Damage = 1;
        AllMods.Add(EProjectileMods::Chain); //
		ActorToFollow = nullptr;
		ExplosionRadius = 10;
		NumberOfPierce =1;
		NumberOfFork = 1;
		NumberOfBounce = 1;
		NumberOfChains = 1;


    }

};		



UCLASS()
class WORTHY_API AWorthyWeapon : public AWorthyItem
{
	GENERATED_BODY()
/** Gun mesh: 1st person view (seen only by self) 

*/
public:

    /** Location on gun mesh where projectiles should spawn. */
    UPROPERTY(VisibleDefaultsOnly, Category = "ItemStats|WeaponStats")
    class USceneComponent *FP_MuzzleLocation;

    //applies all the stats to the projectile
    void ApplyWeaponConfig(FWeaponData &weaponInfo);

public:	
	// Sets default values for this actor's properties
	AWorthyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void StopFire();

    /***Weapon Stuffs*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStats|WeaponStats")
    int32 numberOfProjectiles = 1;

    /** Fires a projectile. */
    void OnFire();

    void ResetWeapon();

    void WeaponTimer();

    bool bIsFIring = false;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStats|WeaponStats")
    float TimeBetweenShots = 0.1f;  //time between each shot fired


    FTimerHandle BurstTimerHandle;  // shot timer

    FTimerHandle SafeFireHandle;

    float LastFireTime;

    int32 currentBurstCounter = 0;

    bool bIsBursting;

    UPROPERTY(EditDefaultsOnly, Category = "ItemStats|weaponStats")
    FWeaponData WeaponStats;

UPROPERTY(EditDefaultsOnly, Category = "ItemStats|weaponStats")
    int32 currentAmmo;

UPROPERTY(EditDefaultsOnly, Category = "ItemStats|weaponStats")
    int32 maxAmmo;

UPROPERTY(EditDefaultsOnly, Category = "ItemStats|weaponStats")
    int32 currentClips;

UPROPERTY(EditDefaultsOnly, Category = "ItemStats|weaponStats")
    int32 maxClips;

    void useAmmo();

UPROPERTY(EditDefaultsOnly, Category = "ItemStats|weaponStats")
    int32 NumberOfBurst = 1;

    /** Gun muzzle's offset from the characters location */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector GunOffset;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AWorthyProjectile> ProjectileClass;

    /** Sound to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)

    class USoundBase *FireSound;

    /** AnimMontage to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    class UAnimMontage *FireAnimation;


protected:


    //plays firing effects
    void PlayEffects();

    void FireProjectile();

    UFUNCTION(Server, Reliable, WithValidation)

    void ServerFire();

    UFUNCTION(Server, Reliable, WithValidation)

    void ServerStopFire();

};
