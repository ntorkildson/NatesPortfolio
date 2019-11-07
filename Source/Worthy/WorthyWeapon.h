// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorthyItem.h"
#include "WorthyWeapon.generated.h"


class USkeletalMeshComponent;


UENUM(BlueprintType)
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

public:

	AWorthyWeapon();

    /** Location on gun mesh where projectiles should spawn. */
    UPROPERTY(VisibleDefaultsOnly, Category = "ItemStats|WeaponStats")
    class USceneComponent *FP_MuzzleLocation;

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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStats|Gameplay")
    FVector GunOffset;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AWorthyProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		int32 NumberOfProjectiles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStats|WeaponStats")
    float TimeBetweenShots = 0.1f;  //time between each shot fired

    /** AnimMontage to play each time we fire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStats|Gameplay")
    class UAnimMontage *FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemStats|Gameplay")
	class USoundBase *FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "ItemStats|weaponStats")
		int32 WeaponRange;


	//assigns projectile stats
	void ApplyWeaponConfig(FWeaponData &weaponInfo);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void StopFire();


    /** Fires a projectile. */
    void OnFire();

protected:

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerFire();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerStopFire();

public:
    void ResetWeapon();

    void WeaponTimer();

    bool bIsFiring = false;

	bool bCanFire = true;




    FTimerHandle BurstTimerHandle;  // shot timer

    FTimerHandle SafeFireHandle;

    float LastFireTime;

    int32 currentBurstCounter = 0;

    bool bIsBursting;

	   
	void OnEquip();


protected:


    //plays firing effects
    void PlayEffects();

    void FireProjectile();

	void FireTrace();



};
