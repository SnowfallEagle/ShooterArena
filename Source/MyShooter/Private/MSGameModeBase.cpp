// MyShooter Game, All Rights Reserved.
#include "MSGameModeBase.h"
#include "Character/MSCharacter.h"
#include "Player/MSPlayerController.h"

AMSGameModeBase::AMSGameModeBase()
{
    DefaultPawnClass = AMSCharacter::StaticClass();
    PlayerControllerClass = AMSPlayerController::StaticClass();
}
