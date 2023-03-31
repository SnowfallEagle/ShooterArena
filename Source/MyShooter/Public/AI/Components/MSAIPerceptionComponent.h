// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "MSAIPerceptionComponent.generated.h"

UCLASS()
class MYSHOOTER_API UMSAIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()

public:
    AActor* GetClosestEnemy() const;

private:
    template<typename T = UAISense>
    AActor* GetBestEnemyBySense(const AAIController* OwnerController, const AActor* OwnerActor) const;

    AActor* FindBestByDistance(const AAIController* OwnerController, const AActor* OwnerActor, const TArray<AActor*>& Actors) const;
};

template<typename T>
inline AActor* UMSAIPerceptionComponent::GetBestEnemyBySense(const AAIController* OwnerController, const AActor* OwnerActor) const
{
    TArray<AActor*> PerceiveActors;
    GetCurrentlyPerceivedActors(T::StaticClass(), PerceiveActors);
    return PerceiveActors.Num() > 0 ? FindBestByDistance(OwnerController, OwnerActor, PerceiveActors) : nullptr;
}
