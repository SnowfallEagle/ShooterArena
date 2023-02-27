// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!bInfinite"))
    int32 Clips;

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

private:
    FAmmoData CurrentAmmo;

public:
    AMSWeapon();

    virtual void StartFire() {}
    virtual void StopFire() {}

    void ChangeClip();
    FORCEINLINE bool CanReload() const { return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0; }

    const FWeaponUIData& GetUIData() const { return UIData; }
    void GetAmmoData(FAmmoData& InCurrentAmmo, FAmmoData& InDefaultAmmo) const;

protected:
    virtual void BeginPlay() override;

    virtual void MakeShot() {}
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    bool MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd) const;

    FORCEINLINE FTransform GetMuzzleTransform() { return WeaponMesh->GetSocketTransform(MuzzleSocketName); }
    AController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    void DecreaseAmmo();

    FORCEINLINE bool IsAmmoEmpty() const { return !CurrentAmmo.bInfinite && CurrentAmmo.Clips <= 0 && IsClipEmpty(); }
    FORCEINLINE bool IsClipEmpty() const { return CurrentAmmo.Bullets <= 0; }
};
