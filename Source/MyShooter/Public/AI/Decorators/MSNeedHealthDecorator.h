// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MSNeedHealthDecorator.generated.h"

UCLASS()
class MYSHOOTER_API UMSNeedHealthDecorator : public UBTDecorator
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthPercent = 0.6f;

public:
    UMSNeedHealthDecorator() { NodeName = "Need Health"; }

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
