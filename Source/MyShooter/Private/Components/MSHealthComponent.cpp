// MyShooter Game, All Rights Reserved.

#include "Components/MSHealthComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AISense_Damage.h"
#include "TimerManager.h"
#include "Core/CoreUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

UMSHealthComponent::UMSHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UMSHealthComponent::TryToAddHealth(float InHealth)
{
    if (IsDead() || IsHealthFull() || InHealth < 0.0f)
    {
        return false;
    }

    SetHealth(Health + InHealth);

    return true;
}

void UMSHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(MaxHealth > 0, TEXT("MaxHealth should be > 0"));

    if (AActor* ComponentOwner = GetOwner())
    {
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &UMSHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UMSHealthComponent::OnTakeRadialDamage);
    }

    SetHealth(MaxHealth);
}

void UMSHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
    float FinalDamage = Damage;

    if (const auto Character = Cast<ACharacter>(DamagedActor))
    {
        const UPhysicalMaterial* PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
        if (const float* Modifier = DamageModifiers.Find(PhysMaterial))
        {
            FinalDamage *= *Modifier;
        }
    }

    UE_LOG(LogHealthComponent, Display, TEXT("Point Damage %f, Final Damage %f, Bone Name: %s"), Damage, FinalDamage, *BoneName.ToString())
    ApplyDamage(FinalDamage, InstigatedBy, DamageCameraShakeClass);
}

void UMSHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
    UE_LOG(LogHealthComponent, Display, TEXT("Radial Damage %f"), Damage);
    ApplyDamage(Damage, InstigatedBy, ProjectileImpactShakeClass);
}

void UMSHealthComponent::SetHealth(float InHealth)
{
    const UWorld* World = GetWorld();

    const float OldHealth = Health;
    Health = FMath::Clamp(InHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health, Health - OldHealth);

    if (IsDead())
    {
        OnDeath.Broadcast();

        if (bAutoHeal && World)
        {
            World->GetTimerManager().ClearTimer(AutoHealTimer);
        }
    }
    else if (IsHealthFull())
    {
        if (bAutoHeal && World)
        {
            World->GetTimerManager().ClearTimer(AutoHealTimer);
        }
    }
}

void UMSHealthComponent::OnAutoHealUpdateTimerFired()
{
    SetHealth(Health + AutoHealModifier);
}

void UMSHealthComponent::PlayCameraShake(TSubclassOf<UCameraShakeBase> ShakeClass)
{
    if (IsDead())
    {
        return;
    }

    const auto* Player = Cast<APawn>(GetOwner());
    if (!Player)
    {
        return;
    }

    const auto* Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager)
    {
        return;
    }

    Controller->PlayerCameraManager->StartCameraShake(ShakeClass);
}

void UMSHealthComponent::ReportDamageEvent(float Damage)
{
    if (LastDamageInstigater)
    {
        AActor* Owner = GetOwner();
        AActor* Instigater = LastDamageInstigater->GetPawn();

        if (Owner && Instigater)
        {
            UAISense_Damage::ReportDamageEvent(
                GetWorld(), Owner, Instigater, Damage, Instigater->GetActorLocation(), Owner->GetActorLocation()
            );
        }
    }
}

void UMSHealthComponent::ApplyDamage(float Damage, AController* Instigater, TSubclassOf<UCameraShakeBase> ShakeClass)
{
    const auto* DamagedPawn = Cast<APawn>(GetOwner());
    if (!DamagedPawn)
    {
        return;
    }

    // TODO: Check if teamkill enabled
    if (Damage <= 0.0f || IsDead() || (Instigater && !FCoreUtils::AreEnemies(DamagedPawn->Controller, Instigater)))
    {
        return;
    }

    LastDamageInstigater = Instigater;

    if (bAutoHeal)
    {
        if (const UWorld* World = GetWorld())
        {
            World->GetTimerManager().SetTimer(AutoHealTimer, this, &UMSHealthComponent::OnAutoHealUpdateTimerFired, AutoHealUpdateTime, true, AutoHealDelayTime);
        }
    }
    SetHealth(Health - Damage);

    ReportDamageEvent(Damage);
    PlayCameraShake(ShakeClass);
}
