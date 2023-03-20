// MyShooter Game, All Rights Reserved.

#include "UI/MSGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

void AMSGameHUD::DrawHUD()
{
    Super::DrawHUD();
}

void AMSGameHUD::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (auto PlayerHUDWidget = CreateWidget<UUserWidget>(World, PlayerHUDWidgetClass))
        {
            PlayerHUDWidget->AddToViewport();
        }
    }
}
