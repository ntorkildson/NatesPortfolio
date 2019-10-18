// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "WorthyCharacter.h"
#include "WorthyProjectile.h"
#include "WorthyItem.h"
#include "WorthyWeapon.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Net/UnrealNetwork.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AWorthyCharacter

AWorthyCharacter::AWorthyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}


void AWorthyCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
    if (Role == ROLE_Authority)
    {
       // EquipWeapon();
    }

    //Mesh1P->SetHiddenInGame(false, false);
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AWorthyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AWorthyCharacter::OnFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AWorthyCharacter::StopFire);


    PlayerInputComponent->BindAction("Drop", IE_Released, this, &AWorthyCharacter::dropWeapon);
    PlayerInputComponent->BindAction("Interact", IE_Released, this, &AWorthyCharacter::interact);


    PlayerInputComponent->BindAction("Spawn", IE_Released, this, &AWorthyCharacter::EquipWeapon);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AWorthyCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AWorthyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWorthyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AWorthyCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AWorthyCharacter::LookUpAtRate);
}


void AWorthyCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AWorthyCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AWorthyCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AWorthyCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AWorthyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AWorthyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AWorthyCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AWorthyCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AWorthyCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AWorthyCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AWorthyCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AWorthyCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void AWorthyCharacter::EquipWeapon()
{
    //TODO: fix mesh equip orientation, projectiles fire correctly, but mesh is sideways...
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FVector derp = GetMesh()->GetSocketLocation(TEXT("GripPoint"));
   FRotator meshROt = GetMesh()->GetSocketRotation(TEXT("GripPoint"));

    CurrentWeapon = GetWorld()->SpawnActor<AWorthyWeapon>(DefaultWeapon, derp, meshROt, SpawnParams);

	
    CurrentWeapon->SetOwner(this);
    CurrentWeapon->Instigator = this;
    //CurrentWeapon->AttachToActor(, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
//	CurrentWeapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));



}


void AWorthyCharacter::dropWeapon()
{
    CurrentWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, false));
}

void AWorthyCharacter::interact()
{
    //send out trace to see whats out there
    //everything should inherit from a base class
    //hit the interactwith funciton
    //do whatever its supposed to do


}



void AWorthyCharacter::OnFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->OnFire();
    }


}

bool AWorthyCharacter::ServerFire_Validate()
{
    return true;
}

void AWorthyCharacter::ServerFire_Implementation()
{
    OnFire();
}

void AWorthyCharacter::StopFire()
{
    //start a timer to allow firing again -disallows left click spamming
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }

}



float
AWorthyCharacter::TakeDamage(float Damage, struct FDamageEvent const &DamageEvent, class AController *EventInstigator,
                             class AActor *DamageCauser)
{
    //TODO: resist and damage reduction algorithm goes here.
    currentHealth -= Damage;
    return currentHealth;
}

void AWorthyCharacter::EquipItem(FName ItemSocketLocation, AWorthyItem *NewItem)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

//    FVector itemLocation = GetMesh1P()->GetSocketLocation(ItemSocketLocation);
 //   FRotator itemRotation = GetMesh1P()->GetSocketRotation(ItemSocketLocation);


    if (NewItem)
    {
        NewItem->SetOwner(this);
        NewItem->Instigator = this;
        NewItem->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemSocketLocation);
        NewItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ItemSocketLocation);

    }
}

void AWorthyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AWorthyCharacter, CurrentWeapon);
    DOREPLIFETIME(AWorthyCharacter, myStats);
    DOREPLIFETIME(AWorthyCharacter, currentHealth);

}	

