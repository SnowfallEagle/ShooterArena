// MyShooter Game, All Rights Reserved.

#include "Weapon/MSRifleWeapon.h"
#include "Components/MSWeaponFXComponent.h"
#include "Components/MSWeaponFlashlightComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Character/MSCharacter.h"

AMSRifleWeapon::AMSRifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<UMSWeaponFXComponent>("WeaponFXComponent");
    check(WeaponFXComponent);

    FlashlightComponent = CreateDefaultSubobject<UMSWeaponFlashlightComponent>("FlashlightComponent");
    check(FlashlightComponent);
    FlashlightComponent->SetupAttachment(GetRootComponent());

    BulletSpread = DefaultBulletSpread;
}

void AMSRifleWeapon::StartFire()
{
    ToggleAllFX(true);
    GetWorldTimerManager().SetTimer(ShotTimer, this, &AMSRifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void AMSRifleWeapon::StopFire()
{
    ToggleAllFX(false);
    GetWorldTimerManager().ClearTimer(ShotTimer);
}

void AMSRifleWeapon::Zoom(bool bToggle)
{
    if (const auto Controller = Cast<APlayerController>(GetController()))
    {
        if (Controller->PlayerCameraManager)
        {
            if (bToggle)
            {
                DefaultFOV = Controller->PlayerCameraManager->GetFOVAngle();
                Controller->PlayerCameraManager->SetFOV(ZoomFOV);
                BulletSpread = DefaultBulletSpread * ZoomSpreadModifier;
            }
            else
            {
                Controller->PlayerCameraManager->SetFOV(DefaultFOV);
                BulletSpread = DefaultBulletSpread;
            }
        }
    }
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
    FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName, FVector::ZeroVector, EAttachLocation::SnapToTarget, false, 1.0f, 1.0f, 0.0f, nullptr, nullptr, false);
    FireAudioComponent->Stop();

    ToggleAllFX(false);
}

void AMSRifleWeapon::MakeShot()
{
    Super::MakeShot();

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

    PlayRecoilShake();
}

bool AMSRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    float RealSpread = BulletSpread;
    if (AMSCharacter* Character = Cast<AMSCharacter>(GetOwner()))
    {
        if (Character->IsRunning())
        {
            RealSpread *= RunningSpreadModifier;
        }
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
        FPointDamageEvent PointDamageEvent;
        PointDamageEvent.HitInfo = HitResult;
        
        Actor->TakeDamage(DamageAmount, PointDamageEvent, GetController(), this);
    }
}

void AMSRifleWeapon::ToggleAllFX(bool bActive)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetVisibility(bActive, true);
        MuzzleFXComponent->SetPaused(!bActive);
    }

    if (FireAudioComponent)
    {
        bActive ? FireAudioComponent->Play() : FireAudioComponent->Stop();
        if (bActive)
        {
            if (!FireAudioComponent->IsActive())
            {
                FireAudioComponent->FadeIn(0.05f);
            }
        }
        else
        {
            FireAudioComponent->IsActive() ? FireAudioComponent->FadeOut(0.05f, 0.1f, EAudioFaderCurve::Logarithmic) : FireAudioComponent->Stop();
        }
    }
}

void AMSRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd) const
{
    if (const UWorld* World = GetWorld())
    {
        if (const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart))
        {
            TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
        }
    }
}

void AMSRifleWeapon::PlayRecoilShake() const
{
    if (const auto* Controller = Cast<APlayerController>(GetController()))
    {
        if (Controller->PlayerCameraManager)
        {
            Controller->PlayerCameraManager->StartCameraShake(RecoilShakeClass);
        }
    }
}
