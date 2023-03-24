// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MSGameModeBase.h"
#include "MSGameHUD.generated.h"

UCLASS()
class MYSHOOTER_API AMSGameHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> GameEndedWidgetClass;

private:
    UPROPERTY()
    TMap<EMatchState, UUserWidget*> Widgets;

    UPROPERTY()
    UUserWidget* CurrentWidget = nullptr;

public:
    void DrawHUD() override;

protected:
    virtual void BeginPlay() override;

private:
    void OnMatchStateChanged(EMatchState NewState);
};
