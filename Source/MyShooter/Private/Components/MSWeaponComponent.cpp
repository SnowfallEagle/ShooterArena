// MyShooter Game, All Rights Reserved.

#include "Components/MSWeaponComponent.h"
#include "Weapon/MSWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/MSEquipFinishedAnimNotify.h"
#include "Animations/MSReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

static constexpr int32 NumWeapons = 2;

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

UMSWeaponComponent::UMSWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMSWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponData.Num() == NumWeapons, TEXT("Character should hold %d weapons"), NumWeapons)

    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void UMSWeaponComponent::EndPlay(EEndPlayReason::Type Reason)
{
    CurrentWeapon = nullptr;

    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(Reason);
}

void UMSWeaponComponent::StartFire()
{
    if (CanFire())
    {
        CurrentWeapon->StartFire();
    }
}

void UMSWeaponComponent::StopFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }
}

void UMSWeaponComponent::NextWeapon()
{
    if (!CanEquip())
    {
        return;
    }

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

bool UMSWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool UMSWeaponComponent::GetWeaponAmmoData(FAmmoData& CurrentAmmo, FAmmoData& DefaultAmmo) const
{
    if (!CurrentWeapon)
    {
        return false;
    }

    CurrentWeapon->GetAmmoData(CurrentAmmo, DefaultAmmo);
    return true;
}

void UMSWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return;
    }

    for (auto& OneWeaponData : WeaponData)
    {
        AMSWeapon* Weapon = GetWorld()->SpawnActor<AMSWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon)
        {
            continue;
        }

        Weapon->SetOwner(Character);
        Weapon->OnClipEmpty.AddUObject(this, &UMSWeaponComponent::OnEmptyClip);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void UMSWeaponComponent::AttachWeaponToSocket(AMSWeapon* Weapon, USceneComponent* Mesh, const FName& SocketName)
{
    if (Weapon && Mesh)
    {
        FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
        Weapon->AttachToComponent(Mesh, AttachmentRules, SocketName);
    }
}

void UMSWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex > WeaponData.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index: %d"), WeaponIndex);
        return;
    }

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return;
    }

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    const FWeaponData* CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) { //
        return Data.WeaponClass == CurrentWeapon->GetClass();                                        //
    });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

    PlayAnimMontage(EquipAnimMontage);
    bEquipAnimInProgress = true;
}

void UMSWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return;
    }

    Character->PlayAnimMontage(AnimMontage);
}

void UMSWeaponComponent::InitAnimations()
{
    auto EquipFinishedNotify = FAnimUtils::FindNotifyByClass<UMSEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &UMSWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is not set"));
        checkNoEntry();
    }

    for (auto& OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = FAnimUtils::FindNotifyByClass<UMSReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (ReloadFinishedNotify)
        {
            ReloadFinishedNotify->OnNotified.AddUObject(this, &UMSWeaponComponent::OnReloadFinished);
        }
        else
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is not set"));
            checkNoEntry();
        }
    }
}

void UMSWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh())
    {
        return;
    }

    bEquipAnimInProgress = false;
}

void UMSWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh())
    {
        return;
    }

    bReloadAnimInProgress = false;
}

void UMSWeaponComponent::ChangeClip()
{
    if (!CanReload())
    {
        return;
    }

    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();

    PlayAnimMontage(CurrentReloadAnimMontage);
    bReloadAnimInProgress = true;
}

