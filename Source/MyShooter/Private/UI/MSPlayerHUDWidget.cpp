// MyShooter Game, All Rights Reserved.

#include "UI/MSPlayerHUDWidget.h"
#include "Components/MSHealthComponent.h"
#include "Components/MSWeaponComponent.h"

float UMSPlayerHUDWidget::GetHealthPercent() const
{
    const APawn* Player = GetOwningPlayerPawn();
    if (!Player)
    {
        return 0.0f;
    }

    const auto Component = Player->GetComponentByClass(UMSHealthComponent::StaticClass());
    const auto HealthComponent = Cast<UMSHealthComponent>(Component);

    return HealthComponent ? HealthComponent->GetHealthPercent() : 0.0f;
}

bool UMSPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const APawn* Player = GetOwningPlayerPawn();
    if (!Player)
    {
        return false;
    }

    const auto Component = Player->GetComponentByClass(UMSWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<UMSWeaponComponent>(Component);
    if (!WeaponComponent)
    {
        return false;
    }

    return WeaponComponent->GetWeaponUIData(UIData);
}
