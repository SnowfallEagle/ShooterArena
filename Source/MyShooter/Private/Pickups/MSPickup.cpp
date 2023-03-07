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

    check(CollisionComponent);

    GenerateRotation();
}

void AMSPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    APawn* Pawn = Cast<APawn>(OtherActor);
    if (!Pawn)
    {
        return;
    }

    if (GivePickupTo(Pawn))
    {
        Hide();
    }
}

bool AMSPickup::GivePickupTo(APawn* Pawn)
{
    return false;
}

void AMSPickup::Hide()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetRootComponent()->SetVisibility(false, true);

    GetWorldTimerManager().SetTimer(RespawnTimer, this, &AMSPickup::Respawn, RespawnTime, false);
}

void AMSPickup::Respawn()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    GetRootComponent()->SetVisibility(true, true);

    GenerateRotation();
}

void AMSPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Handle case when actor may stay on the pickup before it's respawned
    ClearComponentOverlaps();

    AddActorLocalRotation(FRotator(0.0f, RotationYaw * DeltaTime, 0.0f));
}
