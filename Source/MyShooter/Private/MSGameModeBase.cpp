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
    if (UWorld* World = GetWorld())
    {
        for (int32 i = 0; i < NumPlayers - 1; ++i)
        {
            FActorSpawnParameters SpawnInfo;
            SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            const auto Controller = World->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
            RestartPlayer(Controller);
        }
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
                Controller->Reset();
            }

            RestartPlayer(Controller);
        }
    }
}
