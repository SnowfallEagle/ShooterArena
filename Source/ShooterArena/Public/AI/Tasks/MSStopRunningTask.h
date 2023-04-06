// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MSStopRunningTask.generated.h"

UCLASS()
class SHOOTERARENA_API UMSStopRunningTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UMSStopRunningTask() { NodeName = "Stop Running Task"; }

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
