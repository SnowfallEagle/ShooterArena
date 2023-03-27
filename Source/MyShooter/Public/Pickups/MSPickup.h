// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSPickup.generated.h"

class USphereComponent;
class USoundCue;

UCLASS()
class MYSHOOTER_API AMSPickup : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, Category = "Pickup")
    float RespawnTime = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundCue* PickupSound;

private:
    float RotationYaw;

    FTimerHandle RespawnTimer;

public:
    AMSPickup();

    virtual void Tick(float DeltaTime) override;

    FORCEINLINE bool CanBeTaken() const { return !GetWorldTimerManager().IsTimerActive(RespawnTimer); }

protected:
    virtual void BeginPlay() override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
    virtual bool GivePickupTo(APawn* Pawn);

    void Hide();
    void Respawn();
    FORCEINLINE void GenerateRotation() { RotationYaw = FMath::RandRange(100.0f, 200.0f) * (FMath::RandBool() ? 1.0f : -1.0f); }
};
