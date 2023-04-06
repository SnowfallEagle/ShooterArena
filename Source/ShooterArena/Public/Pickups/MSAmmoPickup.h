// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/MSPickup.h"
#include "MSAmmoPickup.generated.h"

class AMSWeapon;

UCLASS()
class SHOOTERARENA_API AMSAmmoPickup : public AMSPickup
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
    int32 Clips = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    TSubclassOf<AMSWeapon> WeaponClass;

private:
    virtual bool GivePickupTo(APawn* Pawn) override;
};
