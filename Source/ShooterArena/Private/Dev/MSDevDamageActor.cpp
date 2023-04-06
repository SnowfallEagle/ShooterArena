// ShooterArena Game, All Rights Reserved.

#include "Dev/MSDevDamageActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AMSDevDamageActor::AMSDevDamageActor()
{
	PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);
}

void AMSDevDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMSDevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (const UWorld* World = GetWorld())
    {
        DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);
        UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, bDoFullDamage);
    }
}
