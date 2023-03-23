// MyShooter Game, All Rights Reserved.

#include "UI/MSGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogMSGameHUD, All, All);

void AMSGameHUD::DrawHUD()
{
    Super::DrawHUD();
}

void AMSGameHUD::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        Widgets.Add(EMatchState::InProgress, CreateWidget<UUserWidget>(World, PlayerHUDWidgetClass));
        Widgets.Add(EMatchState::Pause, CreateWidget<UUserWidget>(World, PauseHUDWidgetClass));

        for (const auto Pair : Widgets)
        {
            if (Pair.Value)
            {
                Pair.Value->AddToViewport();
                Pair.Value->SetVisibility(ESlateVisibility::Hidden);
            }
        }

        if (const auto GameMode = Cast<AMSGameModeBase>(World->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &AMSGameHUD::OnMatchStateChanged);
        }
    }
}

void AMSGameHUD::OnMatchStateChanged(EMatchState NewState)
{
    UE_LOG(LogMSGameHUD, Display, TEXT("Match State changed: %s"), *UEnum::GetValueAsString(NewState));

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    UUserWidget** Widget = Widgets.Find(NewState);
    CurrentWidget = Widget ? *Widget : nullptr;

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }
}
