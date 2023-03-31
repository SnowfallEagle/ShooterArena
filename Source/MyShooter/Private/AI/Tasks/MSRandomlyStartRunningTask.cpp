// MyShooter Game, All Rights Reserved.

#include "AI/Tasks/MSRandomlyStartRunningTask.h"
#include "AIController.h"
#include "AI/MSAICharacter.h"

EBTNodeResult::Type UMSRandomlyStartRunningTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (Probability <= 0.0f || FMath::FRand() > Probability)
    {
        return EBTNodeResult::Succeeded;
    }

    if (const AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (const auto Character = Cast<AMSAICharacter>(AIController->GetPawn()))
        {
            if (FMath::RandRange(0.0f, 1.0f))
            {
                Character->StartRunning();
            }

            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
