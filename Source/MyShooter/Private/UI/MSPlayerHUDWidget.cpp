// MyShooter Game, All Rights Reserved.

#include "UI/MSPlayerHUDWidget.h"
#include "Components/MSHealthComponent.h"
#include "Components/MSWeaponComponent.h"
#include "Components/ProgressBar.h"
#include "Player/MSPlayerCharacter.h"

float UMSPlayerHUDWidget::GetHealthPercent() const
{
    const auto* HealthComponent = GetPlayerComponent<UMSHealthComponent>();
    return HealthComponent ? HealthComponent->GetHealthPercent() : 0.0f;
}

bool UMSPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto* WeaponComponent = GetPlayerComponent<UMSWeaponComponent>();
    return WeaponComponent && WeaponComponent->GetWeaponUIData(UIData);
}

bool UMSPlayerHUDWidget::GetWeaponAmmoData(FAmmoData& CurrentAmmo, FAmmoData& DefaultAmmo) const
{
    const auto* WeaponComponent = GetPlayerComponent<UMSWeaponComponent>();
    return WeaponComponent && WeaponComponent->GetWeaponAmmoData(CurrentAmmo, DefaultAmmo);
}

bool UMSPlayerHUDWidget::IsPlayerAlive() const
{
    const auto* HealthComponent = GetPlayerComponent<UMSHealthComponent>();
    return HealthComponent && !HealthComponent->IsDead();
}

bool UMSPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto* Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

AMSGameModeBase* UMSPlayerHUDWidget::GetGameMode()
{
    if (const UWorld* World = GetWorld())
    {
        return World->GetAuthGameMode<AMSGameModeBase>();
    }
    return nullptr;
}

void UMSPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (APlayerController* Controller = GetOwningPlayer())
    {
        Controller->GetOnNewPawnNotifier().AddUObject(this, &UMSPlayerHUDWidget::OnPawnChanged);
    }
    OnPawnChanged(GetOwningPlayerPawn());
}

void UMSPlayerHUDWidget::OnPawnChanged(APawn* NewPawn)
{
    if (const auto HealthComponent = GetPlayerComponent<UMSHealthComponent>())
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &UMSPlayerHUDWidget::OnHealthChanged);
    }

    if (const auto PlayerCharacter = Cast<AMSPlayerCharacter>(NewPawn))
    {
        PlayerCharacter->OnHit.AddUObject(this, &UMSPlayerHUDWidget::OnHit);
    }

    UpdateHealthBar();
}

void UMSPlayerHUDWidget::OnHit(bool bKilled)
{
    PlayAnimation(HitAnimation);

    if (bKilled)
    {
        PlayAnimation(DeathHitAnimation);
    }
}

void UMSPlayerHUDWidget::OnHealthChanged(float Health, float DeltaHealth)
{
    if (DeltaHealth < 0.0f)
    {
        OnDamageTaken();

        if (!IsAnimationPlaying(DamageAnimation))
        {
            PlayAnimation(DamageAnimation);
        }
    }

    UpdateHealthBar();
}

void UMSPlayerHUDWidget::UpdateHealthBar()
{
    if (HealthBar)
    {
        const float Health = GetHealthPercent();
        HealthBar->SetPercent(Health);
        HealthBar->SetFillColorAndOpacity(Health > ColorThreshold ? HealthyColor : DyingColor);
        HealthBar->SetVisibility(Health > 0.0f && Health < VisibilityThreshold ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }    
}
