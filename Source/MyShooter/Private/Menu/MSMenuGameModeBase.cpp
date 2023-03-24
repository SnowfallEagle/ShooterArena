// MyShooter Game, All Rights Reserved.

#include "Menu/MSMenuGameModeBase.h"
#include "Menu/MSMenuPlayerController.h"
#include "Menu/UI/MSMenuHUD.h"

AMSMenuGameModeBase::AMSMenuGameModeBase()
{
    PlayerControllerClass = AMSMenuPlayerController::StaticClass();
    HUDClass = AMSMenuHUD::StaticClass();
}
