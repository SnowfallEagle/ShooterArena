// ShooterArena Game, All Rights Reserved.

#include "Components/MSAIPerceptionComponent.h"
#include "Components/MSHealthComponent.h"
#include "Core/CoreUtils.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "AIController.h"

AActor* UMSAIPerceptionComponent::GetClosestEnemy() const
{
    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller)
    {
        return nullptr;
    }

    const auto* Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return nullptr;
    }

    if (AActor* BestEnemy = GetBestEnemyBySense<UAISense_Sight>(Controller, Pawn))
    {
        return BestEnemy;
    }
    if (AActor* BestEnemy = GetBestEnemyBySense<UAISense_Damage>(Controller, Pawn))
    {
        return BestEnemy;
    }
    return GetBestEnemyBySense<UAISense_Hearing>(Controller, Pawn);
}

AActor* UMSAIPerceptionComponent::FindBestByDistance(
    const AAIController* OwnerController, const AActor* OwnerActor, const TArray<AActor*>& Actors
) const
{
    float BestDistance = TNumericLimits<float>::Max();
    AActor* BestActor = nullptr;

    for (const auto Actor : Actors)
    {
        const auto* PercievePawn = Cast<APawn>(Actor);
        if (!PercievePawn)
        {
            continue;
        }

        if (!FCoreUtils::AreEnemies(PercievePawn->Controller, OwnerController))
        {
            continue;
        }

        const auto* HealthComponent = FCoreUtils::GetActorComponent<UMSHealthComponent>(Actor);
        if (!HealthComponent || HealthComponent->IsDead())
        {
            continue;
        }

        const float Distance = (Actor->GetActorLocation() - OwnerActor->GetActorLocation()).Size();
        if (Distance < BestDistance)
        {
            BestDistance = Distance;
            BestActor = Actor;
        }
    }

    return BestActor;
}
