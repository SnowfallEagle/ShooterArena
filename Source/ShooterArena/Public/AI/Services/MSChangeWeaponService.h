// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MSChangeWeaponService.generated.h"

UCLASS()
class SHOOTERARENA_API UMSChangeWeaponService : public UBTService
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Probability = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float TimeRateBetweenTicks = 5.0f;

private:
    bool bCanTick = true;
    FTimerHandle Timer;

public:
    UMSChangeWeaponService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
    FORCEINLINE void OnCanTickAgain() { bCanTick = true; }
};
