// MyShooter Game, All Rights Reserved.

#include "Components/MSWeaponComponent.h"
#include "Components/MSWeaponFlashlightComponent.h"
#include "Weapon/MSWeapon.h"
#include "Animations/MSEquipFinishedAnimNotify.h"
#include "Animations/MSReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Core/CoreUtils.h"

static constexpr int32 NumWeapons = 2;

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

UMSWeaponComponent::UMSWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMSWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponData.Num() == NumWeapons, TEXT("Character should hold %d weapons"), NumWeapons);

    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void UMSWeaponComponent::EndPlay(EEndPlayReason::Type Reason)
{
    TurnOff();

    for (const auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); // TODO: Do we need to detach?
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
    else if (CurrentWeapon && CurrentWeapon->IsClipEmpty())
    {
        UGameplayStatics::PlaySound2D(GetWorld(), CurrentWeapon->GetNoAmmoSound());
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

void UMSWeaponComponent::TurnOff()
{
    bEnabled = false;
    
    if (CurrentWeapon)
    {
        StopFire();
        CurrentWeapon->OnUnequipped();
        CurrentWeapon = nullptr;
    }
}

void UMSWeaponComponent::Zoom(bool bToggle)
{
    if (CurrentWeapon)
    {
        CurrentWeapon->Zoom(bToggle);
    }
}

void UMSWeaponComponent::ToggleFlashlight()
{
    if (const auto FlashlightComponent = FCoreUtils::GetActorComponent<UMSWeaponFlashlightComponent>(CurrentWeapon))
    {
        FlashlightComponent->Toggle();
    }
}

bool UMSWeaponComponent::TryToAddAmmo(TSubclassOf<AMSWeapon> WeaponClass, int32 Clips)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponClass))
        {
            return Weapon->TryToAddAmmo(Clips);
        }
    }

    return false;
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

bool UMSWeaponComponent::GetWeaponAmmoData(TSubclassOf<AMSWeapon> WeaponClass, FAmmoData& CurrentAmmo, FAmmoData& DefaultAmmo) const
{
    for (const auto* Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponClass))
        {
            Weapon->GetAmmoData(CurrentAmmo, DefaultAmmo);
            return true;
        }
    }

    return false;
}

void UMSWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    for (const auto& OneWeaponData : WeaponData)
    {
        AMSWeapon* Weapon = World->SpawnActor<AMSWeapon>(OneWeaponData.WeaponClass);
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

    // Unequip current weapon
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        CurrentWeapon->OnUnequipped();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    // Equip and attach next weapon
    CurrentWeapon = Weapons[WeaponIndex];
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

    // Set and play current reload anim montage
    const FWeaponData* CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) { //
        return Data.WeaponClass == CurrentWeapon->GetClass();
    });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    PlayAnimMontage(EquipAnimMontage);
    bEquipAnimInProgress = true;
    bReloadAnimInProgress = false; // Cancel reloading
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
    const auto EquipFinishedNotify = FAnimUtils::FindNotifyByClass<UMSEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &UMSWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is not set"));
        checkNoEntry();
    }

    for (const auto& OneWeaponData : WeaponData)
    {
        const auto ReloadFinishedNotify = FAnimUtils::FindNotifyByClass<UMSReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
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

    if (CurrentWeapon)
    {
        CurrentWeapon->OnEquipped();
    }
}

void UMSWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh())
    {
        return;
    }

    if (CurrentWeapon)
    {
        CurrentWeapon->Reload();
    }
    bReloadAnimInProgress = false;
}

void UMSWeaponComponent::Reload()
{
    if (!CanReload())
    {
        return;
    }

    CurrentWeapon->StopFire();

    PlayAnimMontage(CurrentReloadAnimMontage);
    bReloadAnimInProgress = true;
}

void UMSWeaponComponent::OnEmptyClip(AMSWeapon* Weapon)
{
    if (CurrentWeapon == Weapon) // With reloading animation
    {
        Reload();
    }
    else if (Weapon) // Without animation
    {
        Weapon->Reload();
    }
}
