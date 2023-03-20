// MyShooter Game, All Rights Reserved.

#include "Character/Components/MSRespawnComponent.h"
#include "MSGameModeBase.h"

UMSRespawnComponent::UMSRespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMSRespawnComponent::Respawn(int32 AfterTime)
{
    RespawnTimeLeft = AfterTime;

    if (const UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(RespawnTimer, this, &UMSRespawnComponent::OnRespawnTimerUpdate, 1.0f, true);
    }
}

void UMSRespawnComponent::OnRespawnTimerUpdate()
{
    if (--RespawnTimeLeft <= 0)
    {
        if (const UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(RespawnTimer);

            if (const auto GameMode = Cast<AMSGameModeBase>(World->GetAuthGameMode()))
            {
                GameMode->RequestSpawn(Cast<AController>(GetOwner()));
            }
        }
    }
}
