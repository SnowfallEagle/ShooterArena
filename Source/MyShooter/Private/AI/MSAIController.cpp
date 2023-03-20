// MyShooter Game, All Rights Reserved.

#include "AI/MSAIController.h"
#include "AI/MSAICharacter.h"
#include "Components/MSAIPerceptionComponent.h"
#include "Components/MSRespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AMSAIController::AMSAIController()
{
    MSAIPerceptionComponent = CreateDefaultSubobject<UMSAIPerceptionComponent>("MSAIPerceptionComponent");
    SetPerceptionComponent(*MSAIPerceptionComponent);

    MSRespawnComponent = CreateDefaultSubobject<UMSRespawnComponent>("MSRespawnComponent");

    bWantsPlayerState = true;
}

void AMSAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SetFocus(GetFocusedActor());
}

void AMSAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (const auto* MSCharacter = Cast<AMSAICharacter>(InPawn))
    {
        RunBehaviorTree(MSCharacter->BehaviorTree);
    }
}

AActor* AMSAIController::GetFocusedActor() const
{
    return GetBlackboardComponent() ? Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusedActorKeyName)) : nullptr;
}
