// ShooterArena Game, All Rights Reserved.

#include "AI/Services/MSFindEnemyService.h"
#include "Components/MSAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/CoreUtils.h"
#include "AIController.h"

UMSFindEnemyService::UMSFindEnemyService()
{
    NodeName = "Find Enemy";
}

void UMSFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (const auto BlackboardComponent = OwnerComp.GetBlackboardComponent())
    {
        const auto Controller = OwnerComp.GetAIOwner();
        if (const auto* PerceptionComponent = FCoreUtils::GetActorComponent<UMSAIPerceptionComponent>(Controller))
        {
            BlackboardComponent->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
