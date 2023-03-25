// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UMSGameInstance;
class UMSLevelItemWidget;

UCLASS()
class MYSHOOTER_API UMSMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartButton = nullptr;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton = nullptr;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* LevelItemsBox = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

private:
    TArray<UMSLevelItemWidget*> LevelItemWidgets;
    UMSLevelItemWidget* CurrentLevelItemWidget = nullptr;

protected:
    virtual void NativeOnInitialized() override;

private:
    void InitializeButtons();
    void InitializeLevelItems();

    UMSGameInstance* GetGameInstance() const;

    UFUNCTION()
    void OnLevelSelected(const FLevelData& LevelData);

    UFUNCTION()
    void OnStart();

    UFUNCTION()
    void OnQuit();
};
