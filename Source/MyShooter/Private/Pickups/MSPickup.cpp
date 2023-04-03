// MyShooter Game, All Rights Reserved.

#include "Pickups/MSPickup.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogPickup, All, All);

AMSPickup::AMSPickup()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);

    PickupMeshesContainer = CreateDefaultSubobject<USceneComponent>("PickupMeshContainer");
    PickupMeshesContainer->SetupAttachment(GetRootComponent());

    PickupBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupBaseMesh");
    PickupBaseMesh->SetupAttachment(GetRootComponent());
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
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
    }
}

void AMSPickup::Hide()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    PickupMeshesContainer->SetVisibility(false, true);

    GetWorldTimerManager().SetTimer(RespawnTimer, this, &AMSPickup::Respawn, RespawnTime, false);
}

void AMSPickup::Respawn()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    PickupMeshesContainer->SetVisibility(true, true);

    GenerateRotation();
}

void AMSPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Handle case when actor may stay on the pickup before it's respawned
    ClearComponentOverlaps();

    PickupMeshesContainer->AddLocalRotation(FRotator(0.0f, RotationYaw * DeltaTime, 0.0f));
}
