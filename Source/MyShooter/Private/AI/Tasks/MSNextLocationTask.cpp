// MyShooter Game, All Rights Reserved.

#include "AI/Tasks/MSNextLocationTask.h"
#include "AI/MSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UMSNextLocationTask::UMSNextLocationTask()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type UMSNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const AAIController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

    if (!Controller || !Blackboard)
    {
        return EBTNodeResult::Failed;
    }

    APawn* Pawn = Controller->GetPawn();
    if (!Pawn)
    {
        return EBTNodeResult::Failed;
    }

    const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSystem)
    {
        return EBTNodeResult::Failed;
    }

    FNavLocation NavLocation;
    FVector FromLocation;
    if (bFindFromSelf)
    {
        FromLocation = Pawn->GetActorLocation();
    }
    else
    {
        AActor* FromActor = Cast<AActor>(Blackboard->GetValueAsObject(FromActorKey.SelectedKeyName));
        if (!FromActor)
        {
            return EBTNodeResult::Failed;
        }
        FromLocation = FromActor->GetActorLocation();
    }

    const bool bFound = NavSystem->GetRandomReachablePointInRadius(FromLocation, Radius, NavLocation);
    if (!bFound)
    {
        return EBTNodeResult::Failed;
    }

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);

    return EBTNodeResult::Succeeded;
}
