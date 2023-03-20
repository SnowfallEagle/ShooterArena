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
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogWeapon, Warning, TEXT("Clip is empty"));
        return;
    }

    --CurrentAmmo.Bullets;

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}

void AMSWeapon::ChangeClip()
{
    if (!CurrentAmmo.bInfinite)
    {
        if (CurrentAmmo.Clips <= 0)
        {
            UE_LOG(LogWeapon, Warning, TEXT("No more clips"));
            return;
        }

        --CurrentAmmo.Clips;
    }

    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

bool AMSWeapon::TryToAddAmmo(int32 Clips)
{
    if (IsAmmoFull() || Clips <= 0)
    {
        return false;
    }

    bool bAmmoWasEmpty = IsAmmoEmpty();

    int32 SumClips = CurrentAmmo.Clips + Clips;
    if (SumClips > DefaultAmmo.Clips)
    {
        CurrentAmmo.Clips = DefaultAmmo.Clips;
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    }
    else
    {
        CurrentAmmo.Clips = SumClips;
    }

    if (bAmmoWasEmpty)
    {
        OnClipEmpty.Broadcast(this);
    }

    return true;
}
