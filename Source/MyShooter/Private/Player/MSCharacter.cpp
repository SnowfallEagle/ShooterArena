// MyShooter Game, All Rights Reserved.

#include "Character/MSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/MSWeaponComponent.h"
#include "Components/MSHealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacter, All, All);

AMSCharacter::AMSCharacter(const FObjectInitializer& ObjInit) :
    Super(ObjInit.SetDefaultSubobjectClass<UMSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 60.0f, 100.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<UMSHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOnlyOwnerSee(true);

    WeaponComponent = CreateDefaultSubobject<UMSWeaponComponent>("WeaponComponent");
}

void AMSCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());
    check(GetCapsuleComponent());
    check(GetMesh());

    HealthComponent->OnDeath.AddUObject(this, &AMSCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &AMSCharacter::OnHealthChanged);
    // Set health text first time
    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);

    LandedDelegate.AddDynamic(this, &AMSCharacter::OnGroundLanded);
}

void AMSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AMSCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMSCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AMSCharacter::LookUp);
    PlayerInputComponent->BindAxis("TurnAround", this, &AMSCharacter::TurnAround);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMSCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AMSCharacter::OnEndRunning);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMSCharacter::Jump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UMSWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UMSWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UMSWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UMSWeaponComponent::Reload);
    PlayerInputComponent->BindAction("ToggleFlashlight", IE_Pressed, WeaponComponent, &UMSWeaponComponent::ToggleFlashlight);
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

void AMSCharacter::MoveForward(float Amount)
{
    bMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AMSCharacter::OnDeath()
{
    UE_LOG(LogCharacter, Display, TEXT("Player %s is dead"), *GetName());

    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    SetLifeSpan(LifeSpanOnDeath);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    WeaponComponent->StopFire();
}

void AMSCharacter::OnHealthChanged(float NewHealth, float HealthDelta)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.f"), NewHealth)));
}

void AMSCharacter::OnGroundLanded(const FHitResult& HitResult)
{
    const float ZFallVelocity = -GetVelocity().Z;
    if (ZFallVelocity < LandedDamageVelocity.X)
    {
        return;
    }

    const float Damage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, ZFallVelocity);
    TakeDamage(Damage, FDamageEvent(), nullptr, nullptr);

    UE_LOG(LogCharacter, Display, TEXT("Damage by landing: %f"), Damage);
}