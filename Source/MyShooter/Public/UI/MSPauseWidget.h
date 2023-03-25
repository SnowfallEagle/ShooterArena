// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSPauseWidget.generated.h"

class UButton;

UCLASS()
class MYSHOOTER_API UMSPauseWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ClearPauseButton = nullptr;

    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton = nullptr;

protected:
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnClearPause();

    UFUNCTION()
    void OnMainMenu();
};