// MyShooter Game, All Rights Reserved.

#include "Weapon/MSWeapon.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
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
