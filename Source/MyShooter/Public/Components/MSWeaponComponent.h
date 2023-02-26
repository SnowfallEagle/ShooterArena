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

private:
    UPROPERTY()
    AMSWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    UPROPERTY()
    TArray<AMSWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;

    bool bEquipAnimInProgress = false;
    bool bReloadAnimInProgress = false;

public:
    UMSWeaponComponent();

    void StartFire();
    void StopFire();
    void NextWeapon();
    void Reload() { ChangeClip(); }

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type Reason) override;

private:
    void SpawnWeapons();
    void AttachWeaponToSocket(AMSWeapon* Weapon, USceneComponent* Mesh, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex);

    void PlayAnimMontage(UAnimMontage* AnimMontage);
    void InitAnimations();

    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    FORCEINLINE bool CanEquip() const { return !bEquipAnimInProgress && !bReloadAnimInProgress; }
    FORCEINLINE bool CanFire() const { return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress; }
    FORCEINLINE bool CanReload() const { return CurrentWeapon && CurrentWeapon->CanReload() && !bEquipAnimInProgress && !bReloadAnimInProgress; }

    void ChangeClip();
    void OnEmptyClip() { ChangeClip(); }

    template<typename T>
    T* FindNotifyByClass(UAnimSequenceBase* Anim);
};

