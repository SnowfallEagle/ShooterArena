// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSGameModeBase.h"
#include "MSGameEndedWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class SHOOTERARENA_API UMSGameEndedWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStatBox;

    UPROPERTY(meta = (BindWidget))
    UButton* RestartLevelButton;

    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

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
