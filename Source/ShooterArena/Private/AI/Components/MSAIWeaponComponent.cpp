// ShooterArena Game, All Rights Reserved.

#include "AI/Components/MSAIWeaponComponent.h"

void UMSAIWeaponComponent::StartFire()
{
    if (!CanFireWithReloading())
    {
        NextWeapon();
        return;
    }

    if (CurrentWeapon->IsClipEmpty() && CurrentWeapon->CanReload())
    {
        if (!bReloadAnimInProgress)
        {
            Reload();
        }
    }
    else
    {
        CurrentWeapon->StartFire();
    }
}

void UMSAIWeaponComponent::NextWeapon()
{
    if (!CanEquip())
    {
        return;
    }

    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    while (NextIndex != CurrentWeaponIndex)
    {
        if (Weapons[NextIndex] && !Weapons[NextIndex]->IsAmmoEmpty())
        {
            break;
        }

        NextIndex = (NextIndex + 1) % Weapons.Num();
    }

    if (NextIndex != CurrentWeaponIndex)
    {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}
