// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MSRespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYSHOOTER_API UMSRespawnComponent : public UActorComponent
{
    GENERATED_BODY()

private:
    FTimerHandle RespawnTimer;

    int32 RespawnTimeLeft = 0;

public:
    UMSRespawnComponent();

    void Respawn(int32 AfterTime);

private:
    void OnRespawnTimerUpdate();
};
