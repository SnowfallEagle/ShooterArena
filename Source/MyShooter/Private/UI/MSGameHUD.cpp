// MyShooter Game, All Rights Reserved.

#include "UI/MSGameHUD.h"
#include "Engine/Canvas.h"

void AMSGameHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawCrosshair();
}

void AMSGameHUD::DrawCrosshair()
{
    const TInterval<float> Center((float)Canvas->SizeX * 0.5f, (float)Canvas->SizeY * 0.5f);

    static constexpr float HalfLineSize = 10.0f;
    static constexpr float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
