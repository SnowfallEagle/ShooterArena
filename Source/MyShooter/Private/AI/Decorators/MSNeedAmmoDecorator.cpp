// MyShooter Game, All Rights Reserved.

#include "AI/Decorators/MSNeedAmmoDecorator.h"
#include "AIController.h"
#include "Components/MSWeaponComponent.h"
#include "Core/CoreUtils.h"

bool UMSNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    if (const AAIController* Controller = OwnerComp.GetAIOwner())
    {
        if (const auto* WeaponComponent = FCoreUtils::GetActorComponent<UMSWeaponComponent>(Controller->GetPawn()))
        {
            FAmmoData CurrentAmmo, DefaultAmmo;
            WeaponComponent->GetWeaponAmmoData(WeaponClass, CurrentAmmo, DefaultAmmo);

            if (CurrentAmmo.Clips <= 0)
            {
                return true;
            }
        }
    }

    return false;
}
