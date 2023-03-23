// MyShooter Game, All Rights Reserved.

#include "UI/MSPauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

bool UMSPauseWidget::Initialize()
{
    const bool bRes = Super::Initialize();
    if (bRes && ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &UMSPauseWidget::OnClearPause);
    }

    return bRes;
}

void UMSPauseWidget::OnClearPause()
{
    if (UWorld* World = GetWorld())
    {
        if (AGameModeBase* GameMode = World->GetAuthGameMode())
        {
            GameMode->ClearPause();
        }
    }
}
