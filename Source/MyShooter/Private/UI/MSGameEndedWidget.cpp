// MyShooter Game, All Rights Reserved.

#include "UI/MSGameEndedWidget.h"
#include "UI/MSPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "Player/MSPlayerState.h"
#include "Core/CoreUtils.h"

bool UMSGameEndedWidget::Initialize()
{
    const bool bRes = Super::Initialize();
    if (bRes)
    {
        if (const UWorld* World = GetWorld())
        {
            if (const auto GameMode = Cast<AMSGameModeBase>(World->GetAuthGameMode()))
            {
                GameMode->OnMatchStateChanged.AddUObject(this, &UMSGameEndedWidget::OnMatchStateChanged);
            }
        }
    }

    return bRes;
}

void UMSGameEndedWidget::OnMatchStateChanged(EMatchState NewState)
{
    if (NewState == EMatchState::Ended)
    {
        UpdateStats();
    }
}

void UMSGameEndedWidget::UpdateStats()
{
    struct FPlayerStateAndWidget
    {
        const AMSPlayerState* State;
        UMSPlayerStatRowWidget* Widget;

        FORCEINLINE bool operator<(const FPlayerStateAndWidget& Other) const { return State->GetKills() > Other.State->GetKills(); }
    };

    if (!PlayerStatBox)
    {
        return;
    }
    PlayerStatBox->ClearChildren();

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    TArray<FPlayerStateAndWidget> StatesAndWidgets;
    StatesAndWidgets.Reserve(World->GetNumControllers());

    for (auto It = World->GetControllerIterator(); It; ++It)
    {
        const AController* Controller = It->Get();
        if (!Controller)
        {
            continue;
        }

        const auto* PlayerState = Cast<AMSPlayerState>(Controller->PlayerState);
        if (!PlayerState)
        {
            continue;
        }

        const auto PlayerStatRowWidget = CreateWidget<UMSPlayerStatRowWidget>(World, PlayerStatRowWidgetClass);
        if (!PlayerStatRowWidget)
        {
            continue;
        }

        PlayerStatRowWidget->SetName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(FCoreUtils::IntToText(PlayerState->GetKills()));
        PlayerStatRowWidget->SetDeaths(FCoreUtils::IntToText(PlayerState->GetDeaths()));
        PlayerStatRowWidget->SetTeam(FCoreUtils::IntToText(PlayerState->GetTeamID()));
        PlayerStatRowWidget->TogglePlayerIndicatorVisibility(Controller->IsPlayerController());

        StatesAndWidgets.Emplace(FPlayerStateAndWidget { PlayerState, PlayerStatRowWidget });
    }

    StatesAndWidgets.Sort();
    for (const auto StateAndWidget : StatesAndWidgets)
    {
        PlayerStatBox->AddChild(StateAndWidget.Widget);
    }
}
