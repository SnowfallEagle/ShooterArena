// ShooterArena Game, All Rights Reserved.

#include "AI/Services/MSChangeWeaponService.h"
#include "AIController.h"
#include "Components/MSAIWeaponComponent.h"
#include "Core/CoreUtils.h"

UMSChangeWeaponService::UMSChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void UMSChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (!bCanTick || Probability <= 0.0f || FMath::FRand() > Probability)
    {
        return;
    }

    if (const AAIController* Controller = OwnerComp.GetAIOwner())
    {
        if (APawn* Pawn = Controller->GetPawn())
        {
            if (const auto WeaponComponent = FCoreUtils::GetActorComponent<UMSAIWeaponComponent>(Pawn))
            {
                WeaponComponent->NextWeapon();

                bCanTick = false;
                if (const UWorld* World = GetWorld())
                {
                    World->GetTimerManager().SetTimer(Timer, this, &UMSChangeWeaponService::OnCanTickAgain, TimeRateBetweenTicks, false);
                }
            }
        }
    }
}
