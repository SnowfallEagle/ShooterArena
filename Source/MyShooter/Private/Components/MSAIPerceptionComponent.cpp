// MyShooter Game, All Rights Reserved.

#include "Components/MSAIPerceptionComponent.h"
#include "Components/MSHealthComponent.h"
#include "Core/CoreUtils.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "AIController.h"

AActor* UMSAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PercieveActors;

    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    bool bPercieveBySight = PercieveActors.Num() > 0;

    if (!bPercieveBySight)
    {
        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);
        if (PercieveActors.Num() <= 0)
        {
            return nullptr;
        }
    }

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

    AActor* BestActor = FindBestByDistance(Controller, Pawn, PercieveActors);
    if (!BestActor && bPercieveBySight)
    {
        static constexpr int32 MinArraySize = 4;

        PercieveActors.Init(nullptr, MinArraySize);
        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);

        if (PercieveActors.Num() > 0)
        {
            BestActor = FindBestByDistance(Controller, Pawn, PercieveActors);
        }
    }

    return BestActor;
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
