// MyShooter Game, All Rights Reserved.

#include "Menu/MSMenuPlayerController.h"

void AMSMenuPlayerController::BeginPlay()
{
    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}
