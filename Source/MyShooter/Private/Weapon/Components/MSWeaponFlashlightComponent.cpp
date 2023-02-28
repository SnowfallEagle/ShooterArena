// MyShooter Game, All Rights Reserved.

#include "Weapon/Components/MSWeaponFlashlightComponent.h"

void UMSWeaponFlashlightComponent::BeginPlay()
{
    Super::BeginPlay();

    SetState(false);
}

void UMSWeaponFlashlightComponent::SetState(bool bInEnabled)
{
    bEnabled = bInEnabled;
    SetVisibility(bInEnabled, true);
}
