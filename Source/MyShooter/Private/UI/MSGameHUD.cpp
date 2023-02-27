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

    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }
}
