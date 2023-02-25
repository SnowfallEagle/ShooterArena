// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class MYSHOOTER_API AMSWeapon : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

public:
    AMSWeapon();

    virtual void StartFire() {}
    virtual void StopFire() {}

protected:
    virtual void BeginPlay() override;

    virtual void MakeShot() {}
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    bool MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd) const;

    FTransform GetMuzzleTransform() { return WeaponMesh->GetSocketTransform(MuzzleSocketName); }
    AController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
};
