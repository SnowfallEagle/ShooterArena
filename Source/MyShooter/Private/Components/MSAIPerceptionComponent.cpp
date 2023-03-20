// MyShooter Game, All Rights Reserved.

#include "Components/MSAIPerceptionComponent.h"
#include "Components/MSHealthComponent.h"
#include "Core/CoreUtils.h"
#include "Perception/AISense_Sight.h"
#include "AIController.h"
#include "Player/MSPlayerState.h"

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

    const auto* PlayerState = Controller->GetPlayerState<AMSPlayerState>();
    if (!PlayerState)
    {
        return nullptr;
    }

    const auto* Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return nullptr;
    }

    float BestDistance = MAX_FLT;
    AActor* BestActor = nullptr;

    for (const auto Actor : PercieveActors)
    {
        const auto* PercievePawn = Cast<APawn>(Actor);
        if (!PercievePawn)
        {
            continue;
        }

        if (!FCoreUtils::AreEnemies(PercievePawn->Controller, Controller))
        {
            continue;        
        }

        const auto* HealthComponent = FCoreUtils::GetActorComponent<UMSHealthComponent>(Actor);
        if (!HealthComponent || HealthComponent->IsDead())
        {
            continue;
        }

        const float Distance = (Actor->GetActorLocation() - Pawn->GetActorLocation()).Size();
        if (Distance < BestDistance)
        {
            BestDistance = Distance;
            BestActor = Actor;
        }
    }

    return BestActor;
}
