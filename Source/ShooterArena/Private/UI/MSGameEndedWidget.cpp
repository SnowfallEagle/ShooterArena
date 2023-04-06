// ShooterArena Game, All Rights Reserved.

#include "UI/MSGameEndedWidget.h"
#include "UI/MSPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Player/MSPlayerState.h"
#include "Core/CoreUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MSGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogMSGameEndedWidget, All, All);

void UMSGameEndedWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (const UWorld* World = GetWorld())
    {
        if (const auto GameMode = Cast<AMSGameModeBase>(World->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &UMSGameEndedWidget::OnMatchStateChanged);
        }
    }

    if (RestartLevelButton)
    {
        RestartLevelButton->OnClicked.AddDynamic(this, &UMSGameEndedWidget::OnRestartLevel);
    }

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UMSGameEndedWidget::OnMainMenu);
    }
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
        PlayerStatRowWidget->SetTeamColor(PlayerState->GetTeamColor());
        PlayerStatRowWidget->TogglePlayerIndicatorVisibility(Controller->IsPlayerController());

        StatesAndWidgets.Emplace(FPlayerStateAndWidget{ PlayerState, PlayerStatRowWidget });
    }

    StatesAndWidgets.Sort();
    for (const auto StateAndWidget : StatesAndWidgets)
    {
        PlayerStatBox->AddChild(StateAndWidget.Widget);
    }
}

void UMSGameEndedWidget::OnRestartLevel()
{
    UGameplayStatics::OpenLevel(this, FName(UGameplayStatics::GetCurrentLevelName(this)));
}

void UMSGameEndedWidget::OnMainMenu()
{
    if (const UWorld* World = GetWorld())
    {
        if (const auto* GameInstance = World->GetGameInstance<UMSGameInstance>())
        {
            FName MainMenuLevelName = GameInstance->GetMainMenuLevelName();
            if (MainMenuLevelName.IsNone())
            {
                UE_LOG(LogMSGameEndedWidget, Error, TEXT("There's no MainMenuLevelName in GameInstance"));
                return;
            }

            UGameplayStatics::OpenLevel(this, MainMenuLevelName);
        }
    }
}
