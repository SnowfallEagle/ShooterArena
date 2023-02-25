// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/MSWeapon.h"
#include "MSRifleWeapon.generated.h"

UCLASS()
class MYSHOOTER_API AMSRifleWeapon : public AMSWeapon
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;

private:
    FTimerHandle ShotTimer;

public:
    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
};
