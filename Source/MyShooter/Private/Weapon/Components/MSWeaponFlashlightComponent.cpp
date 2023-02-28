// MyShooter Game, All Rights Reserved.

#include "Weapon/Components/MSWeaponFlashlightComponent.h"

void UMSWeaponFlashlightComponent::BeginPlay()
{
    SetVisibility(false, true);
}

void UMSWeaponFlashlightComponent::ToggleFlashlight()
{
    bEnabled = !bEnabled;
    SetVisibility(bEnabled, true);
}
