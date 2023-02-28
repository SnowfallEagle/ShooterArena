// MyShooter Game, All Rights Reserved.

#include "Weapon/MSWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Gameframework/Character.h"
#include "Gameframework/Controller.h"

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
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets should be >= 0"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips should be >= 0"));

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

bool AMSWeapon::MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd) const
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return false;
    }

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

    return true;
}

AController* AMSWeapon::GetPlayerController() const
{
    const ACharacter* Player = Cast<ACharacter>(GetOwner());
    if (!Player)
    {
        return nullptr;
    }

    return Player->GetController<APlayerController>();
}

bool AMSWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    if (AController* Controller = GetPlayerController())
    {
        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
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
