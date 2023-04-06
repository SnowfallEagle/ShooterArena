// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/MSPickup.h"
#include "MSHealthPickup.generated.h"

UCLASS()
class SHOOTERARENA_API AMSHealthPickup : public AMSPickup
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    float Health = 100.0f;

private:
    virtual bool GivePickupTo(APawn* Pawn) override;
};
