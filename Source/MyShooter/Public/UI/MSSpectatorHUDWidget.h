// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSSpectatorHUDWidget.generated.h"

UCLASS()
class MYSHOOTER_API UMSSpectatorHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetRespawnTimeLeft(int32& TimeLeft);
};
