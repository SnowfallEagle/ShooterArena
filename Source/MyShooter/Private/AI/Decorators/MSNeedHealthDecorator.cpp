// MyShooter Game, All Rights Reserved.

#include "AI/Decorators/MSNeedHealthDecorator.h"
#include "AIController.h"
#include "Components/MSHealthComponent.h"
#include "Core/CoreUtils.h"

bool UMSNeedHealthDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    if (const AAIController* Controller = OwnerComp.GetAIOwner())
    {
        const auto* HealthComponent = FCoreUtils::GetActorComponent<UMSHealthComponent>(Controller->GetPawn());
        if (HealthComponent && !HealthComponent->IsDead() && HealthComponent->GetHealthPercent() <= HealthPercent)
        {
            return true;
        }
    }

    return false;
}
