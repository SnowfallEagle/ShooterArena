// ShooterArena Game, All Rights Reserved.

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
class SHOOTERARENA_API UMSMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* LevelItemsBox;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* LoadingAnimation;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* GameStartSound;

private:
    UPROPERTY()
    TArray<UMSLevelItemWidget*> LevelItemWidgets;

    UPROPERTY()
    UMSLevelItemWidget* CurrentLevelItemWidget;

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
