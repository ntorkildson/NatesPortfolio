// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WorthyCharacter.generated.h"


class UInputComponent;

class AWorthyWeapon;

USTRUCT(BlueprintType)

struct FPlayerStats
{
    GENERATED_BODY()

    UPROPERTY()
    int32 maxHealth;

    UPROPERTY()
    int32 Str;

    UPROPERTY()
    int32 Dex;

    UPROPERTY()
    int32 Int;

    FPlayerStats()
    {
        maxHealth = 100;
        Str = 10;
        Dex = 10;
        Int = 10;
    }

};


UENUM(BlueprintType)
enum class EFireMode : uint8
{
    idle,
    firing,
    reloading,

};


UCLASS(config = Game)

class AWorthyCharacter : public ACharacter
{
    GENERATED_BODY()

    /** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)

    class USkeletalMeshComponent *Mesh1P;


    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))

    class UCameraComponent *FirstPersonCameraComponent;

public:

    AWorthyCharacter();

    void EquipWeapon();

    void interact();

    void dropWeapon();


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWorthyWeapon> DefaultWeapon;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    AWorthyWeapon *CurrentWeapon;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FPlayerStats myStats;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 currentHealth;

    virtual float TakeDamage(float Damage, struct FDamageEvent const &DamageEvent, class AController *EventInstigator,
                             class AActor *DamageCauser) override;

    UFUNCTION(Server, Reliable, WithValidation)

    void ServerFire();

    void StopFire();

    void TakeDamage();

    void CheckResistances();

protected:
    virtual void BeginPlay();

public:
    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera
    )
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera
    )
    float BaseLookUpRate;



    /** Whether to use motion controller location for aiming. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    uint32 bUsingMotionControllers : 1;

protected:

    /** Fires a projectile. */
    void OnFire();

    //plays firing effects
    void PlayEffects();


    /** Resets HMD orientation and position in VR. */
    void OnResetVR();

    /** Handles moving forward/backward */
    void MoveForward(float Val);

    /** Handles stafing movement, left and right */
    void MoveRight(float Val);

    /**
     * Called via input to turn at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate.
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);

    struct TouchData
    {
        TouchData()
        {
            bIsPressed = false;
            Location = FVector::ZeroVector;
        }

        bool bIsPressed;
        ETouchIndex::Type FingerIndex;
        FVector Location;
        bool bMoved;
    };

    void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);

    void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);

    void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);

    TouchData TouchItem;

protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(UInputComponent *InputComponent) override;
    // End of APawn interface

    /*
     * Configures input for touchscreen devices if there is a valid touch interface for doing so
     *
     * @param	InputComponent	The input component pointer to bind controls to
     * @returns true if touch controls were enabled.
     */
    bool EnableTouchscreenMovement(UInputComponent *InputComponent);

public:
    /** Returns Mesh1P subobject **/
    FORCEINLINE class USkeletalMeshComponent *GetMesh1P() const
    { return Mesh1P; }

    /** Returns FirstPersonCameraComponent subobject **/
    FORCEINLINE class UCameraComponent *GetFirstPersonCameraComponent() const
    { return FirstPersonCameraComponent; }

    /** Returns FirstPersonCameraComponent subobject **/
    FORCEINLINE class AWorthyWeapon *GetCurrentWeapon() const
    {
        return CurrentWeapon;
    }

};

