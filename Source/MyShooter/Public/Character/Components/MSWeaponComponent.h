// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/MSWeapon.h"
#include "MSWeaponComponent.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AMSWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYSHOOTER_API UMSWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage* EquipAnimMontage;

    UPROPERTY()
    AMSWeapon* CurrentWeapon;

    int32 CurrentWeaponIndex = 0;

    UPROPERTY()
    TArray<AMSWeapon*> Weapons;

private:
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage;

    bool bEquipAnimInProgress = false;
    bool bReloadAnimInProgress = false;

    bool bEnabled = true;

public:
    UMSWeaponComponent();

    virtual void StartFire();
    virtual void StopFire();
    virtual void NextWeapon();

    void Zoom(bool bToggle);
    void TurnOff();

    FORCEINLINE void Reload() { ChangeClip(); }
    bool TryToAddAmmo(TSubclassOf<AMSWeapon> WeaponClass, int32 Clips);

    void ToggleFlashlight();

    FORCEINLINE bool CanEquip() const { return bEnabled && !bEquipAnimInProgress && !bReloadAnimInProgress; }
    FORCEINLINE bool CanFire() const { return bEnabled && CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress; }
    FORCEINLINE bool CanReload() const { return bEnabled && CurrentWeapon && CurrentWeapon->CanReload() && !bEquipAnimInProgress && !bReloadAnimInProgress; }

    bool GetWeaponUIData(FWeaponUIData& UIData) const;
    bool GetWeaponAmmoData(FAmmoData& CurrentAmmo, FAmmoData& DefaultAmmo) const;
    bool GetWeaponAmmoData(TSubclassOf<AMSWeapon> WeaponClass, FAmmoData& CurrentAmmo, FAmmoData& DefaultAmmo) const;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type Reason) override;

    void EquipWeapon(int32 WeaponIndex);

private:
    void SpawnWeapons();
    void AttachWeaponToSocket(AMSWeapon* Weapon, USceneComponent* Mesh, const FName& SocketName);

    void PlayAnimMontage(UAnimMontage* AnimMontage);
    void InitAnimations();

    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    void ChangeClip();
    void OnEmptyClip(AMSWeapon* Weapon);
};

