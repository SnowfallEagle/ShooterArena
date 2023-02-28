// MyShooter Game, All Rights Reserved.

#include "Weapon/Components/MSWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UMSWeaponFXComponent::UMSWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMSWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult) const
{
    UNiagaraSystem* Effect = DefaultEffect;

    if (HitResult.PhysMaterial.IsValid())
    {
        const UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get();
        if (EffectsMap.Contains(PhysMaterial))
        {
            Effect = EffectsMap[PhysMaterial];
        }
    }

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
}
