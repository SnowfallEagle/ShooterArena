// MyShooter Game, All Rights Reserved.

#include "Pickups/MSPickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPickup, All, All);

AMSPickup::AMSPickup()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);
}

void AMSPickup::BeginPlay()
{
    Super::BeginPlay();
}

void AMSPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    UE_LOG(LogPickup, Display, TEXT("Pickup was taken"));
    Destroy();
}

void AMSPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
