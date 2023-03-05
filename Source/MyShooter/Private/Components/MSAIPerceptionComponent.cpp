// MyShooter Game, All Rights Reserved.

#include "Components/MSAIPerceptionComponent.h"
#include "Components/MSHealthComponent.h"
#include "Core/CoreUtils.h"
#include "Perception/AISense_Sight.h"
#include "AIController.h"

AActor* UMSAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PercieveActors;

    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    if (PercieveActors.Num() <= 0)
    {
        return nullptr;
    }

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller)
    {
        return nullptr;
    }

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return nullptr;
    }

    float BestDistance = MAX_FLT;
    AActor* BestActor = nullptr;

    for (const auto Actor : PercieveActors)
    {
        // TODO: Check if actor is our teammate

        const auto HealthComponent = FCoreUtils::GetActorComponent<UMSHealthComponent>(Actor);
        if (HealthComponent && !HealthComponent->IsDead())
        {
            const float Distance = (Actor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (Distance < BestDistance)
            {
                BestDistance = Distance;
                BestActor = Actor;
            }
        }
    }

    return BestActor;
}
