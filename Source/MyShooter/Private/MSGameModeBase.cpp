// MyShooter Game, All Rights Reserved.

#include "MSGameModeBase.h"
#include "Core/CoreUtils.h"
#include "EngineUtils.h"
#include "Components/MSRespawnComponent.h"
#include "Components/MSWeaponComponent.h"
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

    SetMatchState(EMatchState::InProgress);
}

bool AMSGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const bool bPaused = Super::SetPause(PC, CanUnpauseDelegate);
    if (bPaused)
    {
        SetMatchState(EMatchState::Pause);
    }

    return bPaused;
}

bool AMSGameModeBase::ClearPause()
{
    const bool bCleared = Super::ClearPause();
    if (bCleared)
    {
        SetMatchState(EMatchState::InProgress);    
    }

    return bCleared;
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
            continue;
        }

        RestartPlayer(Controller);
    }
}

void AMSGameModeBase::SetTeamInfo()
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    int32 TeamID = 1;
    int32 BotID = 1;

    for (auto It = World->GetControllerIterator(); It; ++It)
    {
        if (AController* Controller = It->Get())
        {
            const auto PlayerState = Cast<AMSPlayerState>(Controller->PlayerState);
            if (!PlayerState)
            {
                continue;
            }
            PlayerState->SetTeamID(TeamID);
            PlayerState->SetTeamColor(TeamColors[TeamID]);
            PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : FString("Bot") + FString::FromInt(BotID++));

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

void AMSGameModeBase::ReportKill(AController* Killer, AController* Victim)
{
    const auto KillerPlayerState = Killer ? Cast<AMSPlayerState>(Killer->PlayerState) : nullptr;
    const auto VictimPlayerState = Victim ? Cast<AMSPlayerState>(Victim->PlayerState) : nullptr;

    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }

    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }

    StartSpawn(Victim);
}

void AMSGameModeBase::OnRoundUpdate()
{
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
            EndGame();

            UE_LOG(LogMSGameModeBase, Display, TEXT("Round over"));
        }

        LogPlayerStates();
    }
}

void AMSGameModeBase::RequestSpawn(AController* Controller)
{
    ResetPlayer(Controller);
}

void AMSGameModeBase::ResetPlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }

    RestartPlayer(Controller);
    SetCharacterColor(Controller);
}

void AMSGameModeBase::ResetPlayers()
{
    if (const UWorld* World = GetWorld())
    {
        for (auto It = World->GetControllerIterator(); It; ++It)
        {
            ResetPlayer(It->Get());
        }
    }
}

void AMSGameModeBase::LogPlayerStates()
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    for (auto It = World->GetControllerIterator(); It; ++It)
    {
        if (AController* Controller = It->Get())
        {
            if (const auto PlayerState = Cast<AMSPlayerState>(Controller->PlayerState))
            {
                PlayerState->LogInfo();
            }
        }
    }
}

void AMSGameModeBase::StartSpawn(AController* Controller)
{
    if (RoundTimeLeft > (MinRoundTimeToSpawn + RespawnTime))
    {
        if (const auto RespawnComponent = FCoreUtils::GetActorComponent<UMSRespawnComponent>(Controller))
        {
            RespawnComponent->Respawn(RespawnTime);
        }
    }
}

void AMSGameModeBase::EndGame()
{
    if (UWorld* World = GetWorld())
    {
        for (const auto Pawn : TActorRange<APawn>(World))
        {
            if (Pawn)
            {
                Pawn->TurnOff();
                Pawn->DisableInput(nullptr);
            }
        }
    }

    SetMatchState(EMatchState::Ended);
}

void AMSGameModeBase::SetMatchState(EMatchState NewState)
{
    if (MatchState == NewState)
    {
        return;
    }

    MatchState = NewState;
    OnMatchStateChanged.Broadcast(NewState);

    UE_LOG(LogMSGameModeBase, Display, TEXT("State changed: %s"), *UEnum::GetValueAsString(NewState));
}
