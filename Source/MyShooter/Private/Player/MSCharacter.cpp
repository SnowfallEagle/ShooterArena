// MyShooter Game, All Rights Reserved.

#include "Character/MSCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/MSWeaponComponent.h"
#include "Components/MSHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "MSGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacter, All, All);

AMSCharacter::AMSCharacter(const FObjectInitializer& ObjInit) :
    Super(ObjInit.SetDefaultSubobjectClass<UMSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<UMSHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<UMSWeaponComponent>("WeaponComponent");
}

void AMSCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
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

void AMSCharacter::SetCharacterColor(const FLinearColor& Color)
{
    UMaterialInstanceDynamic* MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (MaterialInstance)
    {
        MaterialInstance->SetVectorParameterValue(MaterialColorName, Color);
    }
}

void AMSCharacter::OnDeath()
{
    UE_LOG(LogCharacter, Display, TEXT("Player %s is dead"), *GetName());

    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);

    WeaponComponent->StopFire();

    if (const UWorld* World = GetWorld())
    {
        if (const auto GameMode = Cast<AMSGameModeBase>(World->GetAuthGameMode()))
        {
            GameMode->ReportKill(HealthComponent->GetLastDamageInstigater(), Controller);
        }
    }

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    SetLifeSpan(LifeSpanOnDeath);
}

void AMSCharacter::TurnOff()
{
    Super::TurnOff();

    WeaponComponent->StopFire();
}

void AMSCharacter::OnHealthChanged(float NewHealth, float HealthDelta)
{
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