// MyShooter Game, All Rights Reserved.

#include "Components/MSHealthComponent.h"
#include "GameFramework/Actor.h"
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
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UMSHealthComponent::OnTakeAnyDamage);
    }

    SetHealth(MaxHealth);
}

void UMSHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    auto DamagedPawn = Cast<APawn>(DamagedActor);
    if (!DamagedPawn)
    {
        return;
    }

    // TODO: Check if teamkill enabled
    if (Damage <= 0.0f || IsDead() || !FCoreUtils::AreEnemies(DamagedPawn->Controller, InstigatedBy))
    {
        return;
    }

    LastDamageInstigater = InstigatedBy;

    if (bAutoHeal)
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().SetTimer(AutoHealTimer, this, &UMSHealthComponent::OnAutoHealUpdateTimerFired, AutoHealUpdateTime, true, AutoHealDelayTime);
        }
    }
    SetHealth(Health - Damage);

    PlayCameraShake();
}

void UMSHealthComponent::SetHealth(float InHealth)
{
    UWorld* World = GetWorld();

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

void UMSHealthComponent::PlayCameraShake()
{
    if (IsDead())
    {
        return;
    }

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player)
    {
        return;
    }

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager)
    {
        return;
    }

    Controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
}
