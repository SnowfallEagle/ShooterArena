// ShooterArena Game, All Rights Reserved.

#include "AI/EQS/MSEnemyEnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void UMSEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
    if (const auto* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner))
    {
        const auto* EnemyActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(EnemyActorKeyName));
        UEnvQueryItemType_Actor::SetContextHelper(ContextData, EnemyActor);
    }
}
