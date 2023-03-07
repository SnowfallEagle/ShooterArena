// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MSNeedHealthDecorator.generated.h"

UCLASS()
class MYSHOOTER_API UMSNeedHealthDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    UMSNeedHealthDecorator() { NodeName = "Need Health"; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthPercent = 0.6f;

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
