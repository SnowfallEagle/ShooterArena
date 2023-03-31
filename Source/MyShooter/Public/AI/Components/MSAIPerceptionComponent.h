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
    AActor* FindBestByDistance(const AAIController* OwnerController, const AActor* OwnerActor, const TArray<AActor*>& Actors) const;
};
