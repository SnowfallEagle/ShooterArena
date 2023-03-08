// MyShooter Game, All Rights Reserved.

#include "MSGameModeBase.h"
#include "Character/MSCharacter.h"
#include "Player/MSPlayerController.h"
#include "Player/MSPlayerState.h"
#include "AIController.h"
#include "AI/MSAICharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogMSGameModeBase, All, All);

AMSGameModeBase::AMSGameModeBase()
{
    DefaultPawnClass = AMSCharacter::StaticClass();
    PlayerControllerClass = AMSPlayerController::StaticClass();
    PlayerStateClass = AMSPlayerState::StaticClass();
}

void AMSGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    SetTeamInfo();

    CurrentRound = 1;
    StartRound();
}

UClass* AMSGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AMSGameModeBase::SpawnBots()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    for (int32 i = 0; i < NumPlayers - 1; ++i)
    {
        // Spawn actor
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto Controller = World->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        if (!Controller)
        {
            return;
        }
        RestartPlayer(Controller);
    }
}

void AMSGameModeBase::SetTeamInfo()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    int32 TeamID = 1;

    for (auto It = World->GetControllerIterator(); It; ++It)
    {
        if (AController* Controller = It->Get())
        {
            const auto PlayerState = Cast<AMSPlayerState>(Controller->PlayerState);
            if (!PlayerState)
            {
                return;
            }
            PlayerState->SetTeamID(TeamID);
            PlayerState->SetTeamColor(TeamColors[TeamID]);

            SetCharacterColor(Controller);

            TeamID = (TeamID == 1) ? 2 : 1;
        }
    }
}

void AMSGameModeBase::SetCharacterColor(AController* Controller)
{
    if (!Controller)
    {
        return;
    }

    AMSCharacter* Character = Cast<AMSCharacter>(Controller->GetPawn());
    AMSPlayerState* PlayerState = Cast<AMSPlayerState>(Controller->PlayerState);

    if (Character && PlayerState)
    {
        Character->SetCharacterColor(PlayerState->GetTeamColor());
    }
}

void AMSGameModeBase::StartRound()
{
    ResetPlayers();

    RoundTimeLeft = RoundTime;
    GetWorldTimerManager().SetTimer(RoundTimer, this, &AMSGameModeBase::OnRoundUpdate, 1.0f, true);
}

void AMSGameModeBase::OnRoundUpdate()
{
    UE_LOG(LogMSGameModeBase, Display, TEXT("Current round %d, time left %d"), CurrentRound, RoundTimeLeft);

    if (--RoundTimeLeft <= 0)
    {
        GetWorldTimerManager().ClearTimer(RoundTimer);

        if (++CurrentRound <= NumRounds)
        {
            StartRound();

            UE_LOG(LogMSGameModeBase, Display, TEXT("Next round"));
        }
        else
        {
            UE_LOG(LogMSGameModeBase, Display, TEXT("Round over"));
        }
    }
}

void AMSGameModeBase::ResetPlayers()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    for (auto It = World->GetControllerIterator(); It; ++It)
    {
        if (AController* Controller = It->Get())
        {
            if (APawn* Pawn = Controller->GetPawn())
            {
                Pawn->Reset();
            }

            RestartPlayer(Controller);
            SetCharacterColor(Controller);
        }
    }
}
