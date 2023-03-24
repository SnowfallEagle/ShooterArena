// MyShooter Game, All Rights Reserved.

#include "UI/MSPauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void UMSPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UMSPauseWidget::OnClearPause);
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
