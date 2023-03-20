// MyShooter Game, All Rights Reserved.

#include "Player/MSPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogMSPlayerState, All, All);

void AMSPlayerState::LogInfo()
{
    UE_LOG(LogMSPlayerState, Display, TEXT("TeamID: %d, Kills: %d, Deaths: %d"), TeamID, Kills, Deaths);
}
