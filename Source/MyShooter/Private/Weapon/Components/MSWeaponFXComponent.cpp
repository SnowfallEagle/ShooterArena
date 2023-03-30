// MyShooter Game, All Rights Reserved.

#include "Components/MSWeaponFXComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"

UMSWeaponFXComponent::UMSWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMSWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult) const
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    // Find impact data
    const FImpactData* ImpactData = &DefaultImpactData;

    if (HitResult.PhysMaterial.IsValid())
    {
        if (const FImpactData* FoundImpactData = ImpactDataMap.Find(HitResult.PhysMaterial.Get()))
        {
            ImpactData = FoundImpactData;
        }
    }

    // Spawn niagara effect
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        World,                            //
        ImpactData->NiagaraEffect,        //
        HitResult.ImpactPoint,            //
        HitResult.ImpactNormal.Rotation() //
    );

    // Spawn decal
    UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(
        World,                            //
        ImpactData->DecalData.Material,   //
        ImpactData->DecalData.Size,       //
        HitResult.ImpactPoint,            //
        HitResult.ImpactNormal.Rotation() //
    );

    if (DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactData->DecalData.LifeTime, ImpactData->DecalData.FadeOutTime);

        if (ACharacter* Character = Cast<ACharacter>(HitResult.GetActor()))
        {
            DecalComponent->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, HitResult.BoneName);
        }
    }

    // Play impact sound
    UGameplayStatics::SpawnSoundAtLocation(World, ImpactData->ImpactSound, HitResult.ImpactPoint);
}
