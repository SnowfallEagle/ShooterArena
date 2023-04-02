// MyShooter Game, All Rights Reserved.

#include "Player/MSPlayerCharacter.h"
#include "Components/InputComponent.h"
#include "Components/MSWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AMSPlayerCharacter::AMSPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    check(SpringArmComponent);
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 60.0f, 100.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    check(CameraComponent);
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    check(CameraCollisionComponent);
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void AMSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AMSPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMSPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AMSPlayerCharacter::LookUp);
    PlayerInputComponent->BindAxis("TurnAround", this, &AMSPlayerCharacter::TurnAround);

    PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &AMSPlayerCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &AMSPlayerCharacter::OnEndRunning);
    PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMSPlayerCharacter::Jump);

    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, WeaponComponent, &UMSWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, WeaponComponent, &UMSWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", EInputEvent::IE_Pressed, WeaponComponent, &UMSWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, WeaponComponent, &UMSWeaponComponent::Reload);
    PlayerInputComponent->BindAction("ToggleFlashlight", EInputEvent::IE_Pressed, WeaponComponent, &UMSWeaponComponent::ToggleFlashlight);

    DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", EInputEvent::IE_Pressed, WeaponComponent, &UMSWeaponComponent::Zoom, true);
    PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", EInputEvent::IE_Released, WeaponComponent, &UMSWeaponComponent::Zoom, false);
}

void AMSPlayerCharacter::ReportHit(bool bKilled)
{
    UGameplayStatics::PlaySound2D(GetWorld(), bKilled? DeathHitSound : HitSound);

    OnHit.Broadcast(bKilled);
}

void AMSPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMSPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMSPlayerCharacter::OnCameraCollisionEndOverlap);
}

void AMSPlayerCharacter::OnDeath()
{
    Super::OnDeath();

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void AMSPlayerCharacter::MoveForward(float Amount)
{
    bMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AMSPlayerCharacter::OnCameraCollisionBeginOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult
)
{
    OnCameraOverlap();
}

void AMSPlayerCharacter::OnCameraCollisionEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
)
{
    OnCameraOverlap();
}

void AMSPlayerCharacter::OnCameraOverlap()
{
    const bool bHide = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(bHide);

    TArray<USceneComponent*> Components;
    GetMesh()->GetChildrenComponents(true, Components);
    for (const auto Component : Components)
    {
        if (const auto PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
        {
            PrimitiveComponent->SetOwnerNoSee(bHide);
        }
    }
}
