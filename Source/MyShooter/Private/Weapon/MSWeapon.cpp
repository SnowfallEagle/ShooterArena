// MyShooter Game, All Rights Reserved.

#include "Weapon/MSWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
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
}

void AMSWeapon::Fire()
{
    MakeShot();
}

void AMSWeapon::MakeShot()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);

    FVector TraceStart;
    FVector TraceEnd;
    FHitResult HitResult;
    if (!MakeHit(HitResult, TraceStart, TraceEnd))
    {
        return;
    }

    if (HitResult.bBlockingHit)
    {
        const FVector SocketDirection = SocketTransform.GetRotation().GetForwardVector();
        const FVector SocketToImpact = HitResult.ImpactPoint - SocketTransform.GetLocation();

        if (FVector::DotProduct(SocketDirection, SocketToImpact) >= 0.0f)
        {
            MakeDamage(HitResult);

            DrawDebugLine(World, SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
            DrawDebugSphere(World, HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 3.0f, 0, 1.0f);
        }
    }
    else
    {
        DrawDebugLine(World, SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
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

    if (!GetTraceData(TraceStart, TraceEnd))
    {
        return false;
    }

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

    return true;
}

void AMSWeapon::MakeDamage(FHitResult& HitResult)
{
    if (AActor* Actor = HitResult.GetActor())
    {
        Actor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
    }
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
