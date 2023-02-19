// MyShooter Game, All Rights Reserved.

#include "Player/MSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMSCharacter::AMSCharacter(const FObjectInitializer& ObjInit) :
    Super(ObjInit.SetDefaultSubobjectClass<UMSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AMSCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AMSCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMSCharacter::MoveRight);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMSCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AMSCharacter::OnEndRunning);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMSCharacter::Jump);

    PlayerInputComponent->BindAxis("LookUp", this, &AMSCharacter::LookUp);
    PlayerInputComponent->BindAxis("TurnAround", this, &AMSCharacter::TurnAround);
}

void AMSCharacter::MoveForward(float Amount)
{
    bMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

float AMSCharacter::GetMovementDirection() const
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
