// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/MSWeapon.h"
#include "MSRifleWeapon.generated.h"

class UMSWeaponFXComponent;
class UMSWeaponFlashlightComponent;
class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class MYSHOOTER_API AMSRifleWeapon : public AMSWeapon
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, Category = "VFX")
    UMSWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(VisibleAnywhere, Category = "Accessory")
    UMSWeaponFlashlightComponent* FlashlightComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

private:
    FTimerHandle ShotTimer;

    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    UPROPERTY()
    UAudioComponent* FireAudioComponent;

public:
    AMSRifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

    virtual void OnEquipped() override;
    virtual void OnUnequipped() override;

protected:
    virtual void BeginPlay() override;

    void MakeDamage(FHitResult& HitResult);
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

    void ToggleAllFX(bool bActive);
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};
