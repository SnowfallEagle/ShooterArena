// MyShooter Game, All Rights Reserved.

#include "Weapon/MSWeapon.h"
#include "Character/MSCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Gameframework/Character.h"
#include "Gameframework/Controller.h"
#include "Engine/World.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

AMSWeapon::AMSWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void AMSWeapon::GetAmmoData(FAmmoData& InCurrentAmmo, FAmmoData& InDefaultAmmo) const
{
    InCurrentAmmo = CurrentAmmo;
    InDefaultAmmo = DefaultAmmo;
}

void AMSWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);

    CurrentAmmo = DefaultAmmo;
}

void AMSWeapon::OnUnequipped()
{
    Zoom(false);
}

bool AMSWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

bool AMSWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return false;
    }

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;
    World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

    return true;
}

UNiagaraComponent* AMSWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(
        MuzzleFX,                      //
        WeaponMesh,                    //
        MuzzleSocketName,              //
        FVector::ZeroVector,           //
        FRotator::ZeroRotator,         //
        EAttachLocation::SnapToTarget, //
        true                           //
    );
}

AController* AMSWeapon::GetController() const
{
    const APawn* Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

bool AMSWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto* Character = Cast<AMSCharacter>(GetOwner());
    if (!Character)
    {
        return false;
    }

    if (Character->IsPlayerControlled())
    {
        if (const AController* Controller = GetController())
        {
            Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
            return true;
        }
    }
    else
    {
        ViewLocation = GetMuzzleTransform().GetLocation();
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
        return true;
    }

    return false;
}

void AMSWeapon::DecreaseAmmo()
{
    check(CurrentAmmo.ClipBullets > 0)

    --CurrentAmmo.ClipBullets;

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}

void AMSWeapon::ChangeClip()
{
    if (CurrentAmmo.bInfinite)
    {
        CurrentAmmo.ClipBullets = DefaultAmmo.ClipBullets;
    }
    else
    {
        check(CurrentAmmo.StockBullets > 0);

        CurrentAmmo.StockBullets -= DefaultAmmo.ClipBullets - CurrentAmmo.ClipBullets;
        CurrentAmmo.ClipBullets = DefaultAmmo.ClipBullets;

        if (CurrentAmmo.StockBullets < 0)
        {
            CurrentAmmo.ClipBullets += CurrentAmmo.StockBullets;
            CurrentAmmo.StockBullets = 0;
        }
    }
}

bool AMSWeapon::TryToAddAmmo(int32 Clips)
{
    if (IsAmmoFull() || Clips <= 0)
    {
        return false;
    }

    bool bAmmoWasEmpty = IsAmmoEmpty();

    CurrentAmmo.StockBullets += Clips * DefaultAmmo.ClipBullets;
    if (CurrentAmmo.StockBullets > DefaultAmmo.StockBullets)
    {
        CurrentAmmo.ClipBullets = FMath::Min(CurrentAmmo.ClipBullets + (CurrentAmmo.StockBullets - DefaultAmmo.StockBullets), DefaultAmmo.ClipBullets);
        CurrentAmmo.StockBullets = DefaultAmmo.StockBullets;
    }

    if (bAmmoWasEmpty)
    {
        OnClipEmpty.Broadcast(this);
    }

    return true;
}
