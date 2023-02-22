// MyShooter Game, All Rights Reserved.

#include "Components/MSHealthComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

UMSHealthComponent::UMSHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMSHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* ComponentOwner = GetOwner())
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UMSHealthComponent::OnTakeAnyDamage);
    }

    SetHealth(MaxHealth);
}

void UMSHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
    {
        return;
    }

    if (bAutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(AutoHealTimer, this, &UMSHealthComponent::OnAutoHealUpdateTimerFired, AutoHealUpdateTime, true, AutoHealDelayTime);
    }

    SetHealth(Health - Damage);
}

void UMSHealthComponent::SetHealth(float InHealth)
{
    Health = FMath::Clamp(InHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);

    if (IsDead())
    {
        OnDeath.Broadcast();

        if (bAutoHeal)
        {
            GetWorld()->GetTimerManager().ClearTimer(AutoHealTimer);
        }
    }
    else if (FMath::IsNearlyEqual(Health, MaxHealth))
    {
        if (bAutoHeal)
        {
            GetWorld()->GetTimerManager().ClearTimer(AutoHealTimer);
        }
    }
}

void UMSHealthComponent::OnAutoHealUpdateTimerFired()
{
    SetHealth(Health + AutoHealModifier);
}
