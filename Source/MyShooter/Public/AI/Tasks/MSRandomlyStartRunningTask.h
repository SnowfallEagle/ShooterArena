// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MSRandomlyStartRunningTask.generated.h"

UCLASS()
class MYSHOOTER_API UMSRandomlyStartRunningTask : public UBTTaskNode
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Probability = 0.5f;

public:
    UMSRandomlyStartRunningTask() { NodeName = "Randomly Start Running Task"; }

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
