// MyShooter Game, All Rights Reserved.

#include "Weapon/MSRifleWeapon.h"
#include "Components/MSWeaponFXComponent.h"
#include "Components/MSWeaponFlashlightComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"

AMSRifleWeapon::AMSRifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<UMSWeaponFXComponent>("WeaponFXComponent");
    check(WeaponFXComponent);

    FlashlightComponent = CreateDefaultSubobject<UMSWeaponFlashlightComponent>("FlashlightComponent");
    check(FlashlightComponent);
    FlashlightComponent->SetupAttachment(GetRootComponent());
}

void AMSRifleWeapon::StartFire()
{
    ToggleMuzzleFXVisibility(true);
    GetWorldTimerManager().SetTimer(ShotTimer, this, &AMSRifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void AMSRifleWeapon::StopFire()
{
    ToggleMuzzleFXVisibility(false);
    GetWorldTimerManager().ClearTimer(ShotTimer);
}

void AMSRifleWeapon::OnEquipped()
{
    Super::OnEquipped();

    FlashlightComponent->OnEquipped();
}

void AMSRifleWeapon::OnUnequipped()
{
    Super::OnUnequipped();

    FlashlightComponent->OnUnequipped();
}

void AMSRifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    MuzzleFXComponent = SpawnMuzzleFX();
    if (MuzzleFXComponent)
    {
        ToggleMuzzleFXVisibility(false);
    }
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

    FVector* TraceFXEnd = nullptr;

    if (HitResult.bBlockingHit)
    {
        const FTransform SocketTransform = GetMuzzleTransform();
        const FVector SocketDirection = SocketTransform.GetRotation().GetForwardVector();
        const FVector SocketToImpact = HitResult.ImpactPoint - SocketTransform.GetLocation();

        // TODO: Maybe find another way to handle it
        if (FVector::DotProduct(SocketDirection, SocketToImpact) >= 0.0f)
        {
            MakeDamage(HitResult);

            WeaponFXComponent->PlayImpactFX(HitResult);
            TraceFXEnd = &HitResult.ImpactPoint;
        }
    }
    else
    {
        TraceFXEnd = &TraceEnd;
    }

    if (TraceFXEnd)
    {
        SpawnTraceFX(GetMuzzleTransform().GetLocation(), *TraceFXEnd);
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
        Actor->TakeDamage(DamageAmount, FDamageEvent(), GetController(), this);
    }
}

void AMSRifleWeapon::ToggleMuzzleFXVisibility(bool bVisible)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetVisibility(bVisible, true);
        MuzzleFXComponent->SetPaused(!bVisible);
    }
}

void AMSRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    if (const UWorld* World = GetWorld())
    {
        if (const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart))
        {
            TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
        }
    }
}
