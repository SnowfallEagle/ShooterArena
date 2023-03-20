// MyShooter Game, All Rights Reserved.

#include "Player/MSPlayerController.h"
#include "Components/MSRespawnComponent.h"

AMSPlayerController::AMSPlayerController()
{
    MSRespawnComponent = CreateDefaultSubobject<UMSRespawnComponent>("MSRespawnComponent");
}
