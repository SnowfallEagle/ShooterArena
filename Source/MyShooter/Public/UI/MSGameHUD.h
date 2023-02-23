// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MSGameHUD.generated.h"

UCLASS()
class MYSHOOTER_API AMSGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    void DrawHUD() override;

private:
    void DrawCrosshair();
};
