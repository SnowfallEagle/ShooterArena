// MyShooter Game, All Rights Reserved.

#include "Pickups/MSHealthPickup.h"
#include "Components/MSHealthComponent.h"
#include "Core/CoreUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool AMSHealthPickup::GivePickupTo(APawn* Pawn)
{
    const auto HealthComponent = FCoreUtils::GetActorComponent<UMSHealthComponent>(Pawn);
    return HealthComponent && HealthComponent->TryToAddHealth(Health);
}
