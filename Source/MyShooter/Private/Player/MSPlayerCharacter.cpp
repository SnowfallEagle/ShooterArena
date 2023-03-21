// MyShooter Game, All Rights Reserved.

#include "Player/MSPlayerCharacter.h"
#include "Components/InputComponent.h"
#include "Components/MSWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMSPlayerCharacter::AMSPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 60.0f, 100.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void AMSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AMSPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMSPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AMSPlayerCharacter::LookUp);
    PlayerInputComponent->BindAxis("TurnAround", this, &AMSPlayerCharacter::TurnAround);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMSPlayerCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AMSPlayerCharacter::OnEndRunning);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMSPlayerCharacter::Jump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UMSWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UMSWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UMSWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UMSWeaponComponent::Reload);
    PlayerInputComponent->BindAction("ToggleFlashlight", IE_Pressed, WeaponComponent, &UMSWeaponComponent::ToggleFlashlight);
}

void AMSPlayerCharacter::MoveForward(float Amount)
{
    bMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

float AMSPlayerCharacter::GetMovementDirection() const
{
    const FVector Velocity = GetVelocity();
    if (Velocity.IsZero())
    {
        return 0.0f;
    }

    const FVector ForwardVector = GetActorForwardVector();
    const FVector VelocityNormal = Velocity.GetSafeNormal();

    const float AngleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVector, VelocityNormal)));
    const FVector CrossProduct = FVector::CrossProduct(ForwardVector, VelocityNormal);

    return CrossProduct.IsZero() ? AngleBetween : AngleBetween * FMath::Sign(CrossProduct.Z);
}

