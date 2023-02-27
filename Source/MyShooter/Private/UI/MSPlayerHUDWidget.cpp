// MyShooter Game, All Rights Reserved.

#include "UI/MSPlayerHUDWidget.h"
#include "Components/MSHealthComponent.h"
#include "Components/MSWeaponComponent.h"

float UMSPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = GetPlayerComponent<UMSHealthComponent>();
    return HealthComponent ? HealthComponent->GetHealthPercent() : 0.0f;
}

bool UMSPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = GetPlayerComponent<UMSWeaponComponent>();
    return WeaponComponent ? WeaponComponent->GetWeaponUIData(UIData) : false;
}

bool UMSPlayerHUDWidget::GetWeaponAmmoData(FAmmoData& CurrentAmmo, FAmmoData& DefaultAmmo) const
{
    const auto WeaponComponent = GetPlayerComponent<UMSWeaponComponent>();
    return WeaponComponent ? WeaponComponent->GetWeaponAmmoData(CurrentAmmo, DefaultAmmo) : false;
}

template<typename T>
T* UMSPlayerHUDWidget::GetPlayerComponent() const
{
    const APawn* Player = GetOwningPlayerPawn();
    if (!Player)
    {
        return nullptr;
    }

    const auto Component = Player->GetComponentByClass(T::StaticClass());
    return Cast<T>(Component);
}
