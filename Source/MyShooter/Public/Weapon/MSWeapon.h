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

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float DamageAmount = 10.0f;

public:
    AMSWeapon();

    virtual void Fire();

protected:
    virtual void BeginPlay() override;

    void MakeShot();
    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    bool MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd) const;
    void MakeDamage(FHitResult& HitResult);

    AController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
};
