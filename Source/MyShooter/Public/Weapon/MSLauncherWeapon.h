// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/MSWeapon.h"
#include "MSLauncherWeapon.generated.h"

class AMSProjectile;

UCLASS()
class MYSHOOTER_API AMSLauncherWeapon : public AMSWeapon
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AMSProjectile> ProjectileClass;

public:
    virtual void StartFire() override;

protected:
    virtual void MakeShot() override;
};
