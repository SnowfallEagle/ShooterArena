// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UMSGameInstance;
class UMSLevelItemWidget;
class USoundCue;

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

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* LoadingAnimation = nullptr;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* GameStartSound = nullptr;

private:
    UPROPERTY()
    TArray<UMSLevelItemWidget*> LevelItemWidgets;

    UPROPERTY()
    UMSLevelItemWidget* CurrentLevelItemWidget = nullptr;

protected:
    virtual void NativeOnInitialized() override;
    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation);

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
