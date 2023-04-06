// ShooterArena Game, All Rights Reserved.

#include "AI/Tasks/MSRandomlyStartRunningTask.h"
#include "AIController.h"
#include "AI/MSAICharacter.h"

EBTNodeResult::Type UMSRandomlyStartRunningTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (const AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (const auto Character = Cast<AMSAICharacter>(AIController->GetPawn()))
        {
            Probability <= 0.0f || FMath::FRand() > Probability ? Character->StopRunning() : Character->StartRunning();
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
