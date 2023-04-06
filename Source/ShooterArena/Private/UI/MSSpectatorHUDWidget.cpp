// ShooterArena Game, All Rights Reserved.

#include "UI/MSSpectatorHUDWidget.h"
#include "Core/CoreUtils.h"
#include "Components/MSRespawnComponent.h"

bool UMSSpectatorHUDWidget::GetRespawnTimeLeft(int32& TimeLeft)
{
    const auto* RespawnComponent = FCoreUtils::GetActorComponent<UMSRespawnComponent>(GetOwningPlayer());
    if (RespawnComponent && RespawnComponent->IsRespawning())
    {
        TimeLeft = RespawnComponent->GetRespawnTimeLeft();
        return true;
    }
    return false;
}
