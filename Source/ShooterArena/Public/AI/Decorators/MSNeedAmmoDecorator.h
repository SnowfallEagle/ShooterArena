// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MSNeedAmmoDecorator.generated.h"

class AMSWeapon;

UCLASS()
class SHOOTERARENA_API UMSNeedAmmoDecorator : public UBTDecorator
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TSubclassOf<AMSWeapon> WeaponClass;

public:
    UMSNeedAmmoDecorator() { NodeName = "Need Ammo"; }

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
