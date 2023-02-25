// MyShooter Game, All Rights Reserved.

#include "Weapon/MSLauncherWeapon.h"
#include "Weapon/MSProjectile.h"
#include "DrawDebugHelpers.h"

void AMSLauncherWeapon::StartFire()
{
    MakeShot();
}

void AMSLauncherWeapon::MakeShot()
{
    if (IsAmmoEmpty())
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    FVector TraceStart;
    FVector TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        return;
    }

    FHitResult HitResult;
    if (!MakeHit(HitResult, TraceStart, TraceEnd))
    {
        return;
    }
    FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;

    const FTransform SocketTransform = GetMuzzleTransform();
    const FVector SocketDirection = SocketTransform.GetRotation().GetForwardVector();
    const FVector SocketToEnd = EndPoint - SocketTransform.GetLocation();

    // TODO: Maybe find another way to handle it
    if (FVector::DotProduct(SocketDirection, SocketToEnd) < 0.0f)
    {
        return;
    }

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleTransform().GetLocation());
    AMSProjectile* Projectile = World->SpawnActorDeferred<AMSProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(SocketToEnd.GetSafeNormal());
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }

    DecreaseAmmo();
}

