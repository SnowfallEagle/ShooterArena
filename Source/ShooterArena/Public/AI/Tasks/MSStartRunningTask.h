// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MSStartRunningTask.generated.h"

UCLASS()
class SHOOTERARENA_API UMSStartRunningTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UMSStartRunningTask() { NodeName = "Start Running Task"; }

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
