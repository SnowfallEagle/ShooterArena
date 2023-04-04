// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UMSWeaponFXComponent;

UCLASS()
class MYSHOOTER_API AMSProjectile : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    UMSWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageRadius = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool bDoFullDamage = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float LifeSeconds = 5.0f;

private:
    FVector ShotDirection;

public:
    AMSProjectile();

    FORCEINLINE void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnProjectileHit(
        UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit
    );

    AController* GetController() const;
};
