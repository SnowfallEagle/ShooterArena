// MyShooter Game, All Rights Reserved.

#include "Components/MSWeaponComponent.h"
#include "Weapon/MSWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/MSEquipFinishedAnimNotify.h"
#include "Animations/MSReloadFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

UMSWeaponComponent::UMSWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMSWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

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
    auto EquipFinishedNotify = FindNotifyByClass<UMSEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &UMSWeaponComponent::OnEquipFinished);
    }

    for (auto& OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = FindNotifyByClass<UMSReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (ReloadFinishedNotify)
        {
            ReloadFinishedNotify->OnNotified.AddUObject(this, &UMSWeaponComponent::OnReloadFinished);
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

template<typename T>
T* UMSWeaponComponent::FindNotifyByClass(UAnimSequenceBase* Anim)
{
    if (!Anim)
    {
        return nullptr;
    }

    for (auto& NotifyEvent : Anim->Notifies)
    {
        T* AnimNotify = Cast<T>(NotifyEvent.Notify);
        if (AnimNotify)
        {
            return AnimNotify;
        }
    }

    return nullptr;
}
