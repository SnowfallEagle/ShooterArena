// MyShooter Game, All Rights Reserved.

#include "UI/MSPauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MSGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogMSPauseWidget, All, All);

void UMSPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UMSPauseWidget::OnClearPause);
    }

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UMSPauseWidget::OnMainMenu);
    }
}

void UMSPauseWidget::OnClearPause()
{
    if (const UWorld* World = GetWorld())
    {
        if (AGameModeBase* GameMode = World->GetAuthGameMode())
        {
            GameMode->ClearPause();
        }
    }
}

void UMSPauseWidget::OnMainMenu()
{
    if (const UWorld* World = GetWorld())
    {
        if (const auto* GameInstance = World->GetGameInstance<UMSGameInstance>())
        {
            FName MainMenuLevelName = GameInstance->GetMainMenuLevelName();
            if (MainMenuLevelName.IsNone())
            {
                UE_LOG(LogMSPauseWidget, Error, TEXT("There's no MainMenuLevelName in GameInstance"));
                return;
            }

            UGameplayStatics::OpenLevel(this, MainMenuLevelName);
        }
    }
}
