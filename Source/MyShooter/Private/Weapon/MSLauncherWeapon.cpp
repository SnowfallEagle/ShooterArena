// MyShooter Game, All Rights Reserved.

#include "Weapon/MSLauncherWeapon.h"
#include "Weapon/MSProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void AMSLauncherWeapon::StartFire()
{
    MakeShot();
}

void AMSLauncherWeapon::MakeShot()
{
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

    if (FVector::DotProduct(SocketDirection, SocketToEnd) >= 0.0f)
    {
        const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleTransform().GetLocation());
        AActor* Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);
        // TODO: Set projectile params
        UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
    }
}

