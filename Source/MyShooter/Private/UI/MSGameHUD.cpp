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

void AMSGameHUD::DrawCrosshair_DEPRECATED()
{
    const TInterval<float> Center((float)Canvas->SizeX * 0.5f, (float)Canvas->SizeY * 0.5f);

    static constexpr float HalfLineSize = 10.0f;
    static constexpr float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
