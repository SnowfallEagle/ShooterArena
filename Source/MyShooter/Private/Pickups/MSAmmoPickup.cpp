// MyShooter Game, All Rights Reserved.

#include "Pickups/MSAmmoPickup.h"
#include "Components/MSHealthComponent.h"
#include "Components/MSWeaponComponent.h"
#include "Core/CoreUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool AMSAmmoPickup::GivePickupTo(APawn* Pawn)
{
    auto HealthComponent = FCoreUtils::GetActorComponent<UMSHealthComponent>(Pawn);
    if (!HealthComponent || HealthComponent->IsDead())
    {
        return false;
    }

    auto WeaponComponent = FCoreUtils::GetActorComponent<UMSWeaponComponent>(Pawn);
    return WeaponComponent && WeaponComponent->TryToAddAmmo(WeaponClass, Clips);
}
