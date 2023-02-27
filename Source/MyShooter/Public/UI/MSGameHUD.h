// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MSGameHUD.generated.h"

UCLASS()
class MYSHOOTER_API AMSGameHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

public:
    void DrawHUD() override;

protected:
    virtual void BeginPlay() override;

private:
    void DrawCrosshair_DEPRECATED();
};
