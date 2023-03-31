// MyShooter Game, All Rights Reserved.

#include "AI/Tasks/MSStopRunningTask.h"
#include "AI/MSAICharacter.h"
#include "AIController.h"

EBTNodeResult::Type UMSStopRunningTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (const AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (const auto Character = Cast<AMSAICharacter>(AIController->GetPawn()))
        {
            Character->StopRunning();
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
