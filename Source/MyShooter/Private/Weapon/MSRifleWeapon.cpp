// MyShooter Game, All Rights Reserved.

#include "Weapon/MSRifleWeapon.h"
#include "Weapon/Components/MSWeaponFXComponent.h"
#include "Weapon/Components/MSWeaponFlashlightComponent.h"
#include "DrawDebugHelpers.h"

AMSRifleWeapon::AMSRifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<UMSWeaponFXComponent>("WeaponFXComponent");

    FlashlightComponent = CreateDefaultSubobject<UMSWeaponFlashlightComponent>("FlashlightComponent");
    FlashlightComponent->SetupAttachment(GetRootComponent());
}

void AMSRifleWeapon::StartFire()
{
    GetWorldTimerManager().SetTimer(ShotTimer, this, &AMSRifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void AMSRifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimer);
}

void AMSRifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
    check(FlashlightComponent);
}

void AMSRifleWeapon::MakeShot()
{
    FVector TraceStart;
    FVector TraceEnd;
    FHitResult HitResult;
    if (IsAmmoEmpty() || !GetTraceData(TraceStart, TraceEnd) || !MakeHit(HitResult, TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    const FTransform SocketTransform = GetMuzzleTransform();

    if (HitResult.bBlockingHit)
    {
        const FVector SocketDirection = SocketTransform.GetRotation().GetForwardVector();
        const FVector SocketToImpact = HitResult.ImpactPoint - SocketTransform.GetLocation();

        // TODO: Maybe find another way to handle it
        if (FVector::DotProduct(SocketDirection, SocketToImpact) >= 0.0f)
        {
            MakeDamage(HitResult);

            WeaponFXComponent->PlayImpactFX(HitResult);

            //DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
            //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 3.0f, 0, 1.0f);
        }
    }
    else
    {
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }

    DecreaseAmmo();
}

bool AMSRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    TraceStart = ViewLocation;
    const float HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

void AMSRifleWeapon::MakeDamage(FHitResult& HitResult)
{
    if (AActor* Actor = HitResult.GetActor())
    {
        Actor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
    }
}
