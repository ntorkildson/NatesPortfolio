// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WorthyCharacter.generated.h"


class UInputComponent;

class AWorthyWeapon;

class AWorthyItem;



//Genotypes are basically just whatever stats we want to the actor to have.
USTRUCT(BlueprintType)
struct FGenotype
{

	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 Str;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 Dex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 Int;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 mutationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int32 maxPertubation;


	FGenotype()
	//FGenotype(TArray<float> inputWeights, float inputFitness) : weights(inputWeights), fitness(inputFitness)
	{
		maxHealth = 100;
		Str = 10;
		Dex = 10;
		Int = 10;
		mutationRate = 1;
		maxPertubation = 10;

		
	
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


		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:

    AWorthyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FGenotype myStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;


    void EquipWeapon();

    void EquipItem(FName ItemSocketLocation, AWorthyItem* NewItem);


    void Interact();

	AWorthyItem* GetClosestItem();

    void dropWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<TSubclassOf<AWorthyItem>> PlayerInventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory | Weapon")
    TSubclassOf<AWorthyWeapon> DefaultWeapon;


    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Inventory | Weapon")
    AWorthyWeapon *CurrentWeapon;




    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Stats")
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
	virtual void Tick(float DeltaTime) override;

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
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

