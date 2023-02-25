// MyShooter Game, All Rights Reserved.

#include "Weapon/MSProjectile.h"
#include "Components/SphereComponent.h"

AMSProjectile::AMSProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    SetRootComponent(CollisionComponent);
}

void AMSProjectile::BeginPlay()
{
    Super::BeginPlay();
}
