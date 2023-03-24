// MyShooter Game, All Rights Reserved.

#include "Player/MSPlayerController.h"
#include "Components/MSRespawnComponent.h"

AMSPlayerController::AMSPlayerController()
{
    MSRespawnComponent = CreateDefaultSubobject<UMSRespawnComponent>("MSRespawnComponent");
}

void AMSPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (const UWorld* World = GetWorld())
    {
        if (const auto GameMode = Cast<AMSGameModeBase>(World->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &AMSPlayerController::OnMatchStateChanged);
        }
    }
}

void AMSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent)
    {
        return;
    }

    InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &AMSPlayerController::OnPause);
}

void AMSPlayerController::OnPause()
{
    if (const UWorld* World = GetWorld())
    {
        if (AGameModeBase* GameMode = World->GetAuthGameMode())
        {
            GameMode->SetPause(this);
        }
    }
}

void AMSPlayerController::OnMatchStateChanged(EMatchState NewState)
{
    if (NewState == EMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else if (NewState == EMatchState::Pause || NewState == EMatchState::Ended)
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}
