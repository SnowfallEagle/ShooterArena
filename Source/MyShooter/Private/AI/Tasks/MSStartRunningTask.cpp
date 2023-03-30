// MyShooter Game, All Rights Reserved.

#include "AI/Tasks/MSStartRunningTask.h"
#include "AI/MSAICharacter.h"
#include "AIController.h"

EBTNodeResult::Type UMSStartRunningTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (const AAIController* AIController = OwnerComp.GetAIOwner())
    {
        if (const auto Character = Cast<AMSAICharacter>(AIController->GetPawn()))
        {
            Character->StartRunning();
            return Super::ExecuteTask(OwnerComp, NodeMemory);
        }
    }

    return EBTNodeResult::Failed;
}
