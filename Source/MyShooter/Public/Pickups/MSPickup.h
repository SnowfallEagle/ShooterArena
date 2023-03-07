// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSPickup.generated.h"

class USphereComponent;

UCLASS()
class MYSHOOTER_API AMSPickup : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    USphereComponent* CollisionComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Pickup")
    float RespawnTime = 5.0f;

private:
    float RotationYaw;

public:
    AMSPickup();

public:
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
    virtual bool GivePickupTo(APawn* Pawn);

    void Hide();
    void Respawn();
    FORCEINLINE void GenerateRotation() { RotationYaw = FMath::RandRange(100.0f, 200.0f) * (FMath::RandBool() ? 1.0f : -1.0f); }
};
