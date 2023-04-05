// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, AMSWeapon*);

class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundCue;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 ClipBullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!bInfinite"))
    int32 StockBullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool bInfinite;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrosshairIcon;
};

UCLASS()
class MYSHOOTER_API AMSWeapon : public AActor
{
    GENERATED_BODY()

public:
    FOnClipEmptySignature OnClipEmpty;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo = { 15, 10, false };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* MuzzleFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* FireSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* NoAmmoSound;

private:
    FAmmoData CurrentAmmo;

public:
    AMSWeapon();

    virtual void StartFire() {}
    virtual void StopFire() {}

    virtual void OnEquipped() {}
    virtual void OnUnequipped();

    virtual void Zoom(bool bToggle) {}

    void Reload();
    bool TryToAddAmmo(int32 Clips);

    FORCEINLINE bool CanReload() const { return CurrentAmmo.ClipBullets < DefaultAmmo.ClipBullets && CurrentAmmo.StockBullets > 0; }
    FORCEINLINE bool IsAmmoEmpty() const { return !CurrentAmmo.bInfinite && CurrentAmmo.StockBullets <= 0 && IsClipEmpty(); }
    FORCEINLINE bool IsClipEmpty() const { return CurrentAmmo.ClipBullets <= 0; }
    FORCEINLINE bool IsAmmoFull() const { return CurrentAmmo.ClipBullets == DefaultAmmo.ClipBullets && CurrentAmmo.StockBullets == DefaultAmmo.StockBullets; };

    FORCEINLINE USoundCue* GetNoAmmoSound() const { return NoAmmoSound; }

    FORCEINLINE const FWeaponUIData& GetUIData() const { return UIData; }
    void GetAmmoData(FAmmoData& InCurrentAmmo, FAmmoData& InDefaultAmmo) const;

protected:
    virtual void BeginPlay() override;

    virtual void MakeShot();
    bool MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
    void DecreaseAmmo();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    UNiagaraComponent* SpawnMuzzleFX();

    AController* GetController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FORCEINLINE FTransform GetMuzzleTransform() const { return WeaponMesh->GetSocketTransform(MuzzleSocketName); }
};
