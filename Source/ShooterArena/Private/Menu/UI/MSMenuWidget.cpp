// ShooterArena Game, All Rights Reserved.

#include "Menu/UI/MSMenuWidget.h"
#include "Menu/UI/MSLevelItemWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "MSGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogMSMenuWidget, All, All);

void UMSMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitializeButtons();
    InitializeLevelItems();
}

void UMSMenuWidget::InitializeButtons()
{
    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UMSMenuWidget::OnStart);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UMSMenuWidget::OnQuit);
    }
}

void UMSMenuWidget::InitializeLevelItems()
{
    if (!LevelItemsBox)
    {
        return;
    }

    const auto GameInstance = GetGameInstance();
    if (!GameInstance)
    {
        return;
    }

    const TArray<FLevelData>& LevelsData = GameInstance->GetLevelsData();
    checkf(LevelsData.Num() > 0, TEXT("UMSGameInstance::LevelsData must not be empty"));

    LevelItemsBox->ClearChildren();
    for (const auto& Level : LevelsData)
    {
        if (const auto Widget = CreateWidget<UMSLevelItemWidget>(GetWorld(), LevelItemWidgetClass))
        {
            Widget->SetLevelData(Level);
            Widget->OnLevelSelected.AddUObject(this, &UMSMenuWidget::OnLevelSelected);

            LevelItemsBox->AddChild(Widget);
            LevelItemWidgets.Add(Widget);
        }
    }

    const FLevelData& StartupLevel = GameInstance->GetStartupLevel();
    OnLevelSelected(StartupLevel.LevelName.IsNone() ? LevelsData[0] : StartupLevel);
}

void UMSMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation == LoadingAnimation)
    {
        if (const auto GameInstance = GetGameInstance())
        {
            UGameplayStatics::OpenLevel(this, GameInstance->GetStartupLevel().LevelName);
        }
    }
}

UMSGameInstance* UMSMenuWidget::GetGameInstance() const
{
    if (const UWorld* World = GetWorld())
    {
        return World->GetGameInstance<UMSGameInstance>();
    }
    return nullptr;
}

void UMSMenuWidget::OnLevelSelected(const FLevelData& LevelData)
{
    if (const auto GameInstance = GetGameInstance())
    {
        GameInstance->SetStartupLevel(LevelData);

        for (const auto Widget : LevelItemWidgets)
        {
            if (Widget && Widget->GetLevelData().LevelName == LevelData.LevelName)
            {
                if (CurrentLevelItemWidget)
                {
                    CurrentLevelItemWidget->ToggleLevelSelected(false);
                }
                Widget->ToggleLevelSelected(true);

                CurrentLevelItemWidget = Widget;
                return;
            }
        }
    }
}

void UMSMenuWidget::OnStart()
{
    if (!IsAnimationPlaying(LoadingAnimation))
    {
        PlayAnimation(LoadingAnimation);
        UGameplayStatics::PlaySound2D(GetWorld(), GameStartSound);
    }
}

void UMSMenuWidget::OnQuit()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
