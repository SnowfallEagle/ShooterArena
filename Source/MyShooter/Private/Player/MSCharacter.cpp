// MyShooter Game, All Rights Reserved.

#include "Player/MSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMSCharacter::AMSCharacter()
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

    PlayerInputComponent->BindAxis("Run", this, &AMSCharacter::Run);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMSCharacter::Jump);

    PlayerInputComponent->BindAxis("LookUp", this, &AMSCharacter::LookUp);
    PlayerInputComponent->BindAxis("TurnAround", this, &AMSCharacter::TurnAround);
}
