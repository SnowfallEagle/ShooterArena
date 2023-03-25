// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSGameModeBase.h"
#include "MSGameEndedWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class MYSHOOTER_API UMSGameEndedWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStatBox = nullptr;

    UPROPERTY(meta = (BindWidget))
    UButton* RestartLevelButton = nullptr;

    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton = nullptr;

protected:
    virtual void NativeOnInitialized() override;

private:
    void OnMatchStateChanged(EMatchState NewState);
    void UpdateStats();

    UFUNCTION()
    void OnRestartLevel();

    UFUNCTION()
    void OnMainMenu();
};