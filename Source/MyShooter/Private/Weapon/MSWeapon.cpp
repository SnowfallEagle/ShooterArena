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
    --CurrentAmmo.Bullets;

    LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        ChangeClip();
    }
}

void AMSWeapon::ChangeClip()
{
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    if (!CurrentAmmo.bInfinite)
    {
        --CurrentAmmo.Clips;
        UE_LOG(LogWeapon, Display, TEXT("--- Change Clip ---"));
    }
}

void AMSWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.bInfinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogWeapon, Display, TEXT("%s"), *AmmoInfo);
}
