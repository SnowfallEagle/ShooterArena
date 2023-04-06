// ShooterArena Game, All Rights Reserved.

#include "AI/Services/MSFireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/MSWeaponComponent.h"
#include "Components/MSHealthComponent.h"
#include "Core/CoreUtils.h"

UMSFireService::UMSFireService()
{
    NodeName = "Fire";
}

void UMSFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    bool bHasAim = false;

    if (const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
    {
        if (AActor* Enemy = Cast<AActor>(Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName)))
        {
            const auto* HealthComponent = FCoreUtils::GetActorComponent<UMSHealthComponent>(Enemy);
            if (HealthComponent && !HealthComponent->IsDead())
            {
                bHasAim = true;
            }
        }
    }

    if (const AAIController* Controller = OwnerComp.GetAIOwner())
    {
        const auto WeaponComponent = FCoreUtils::GetActorComponent<UMSWeaponComponent>(Controller->GetPawn());
        const auto* HealthComponent = FCoreUtils::GetActorComponent<UMSHealthComponent>(Controller->GetPawn());

        if (WeaponComponent && HealthComponent)
        {
            bHasAim && !HealthComponent->IsDead() ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
