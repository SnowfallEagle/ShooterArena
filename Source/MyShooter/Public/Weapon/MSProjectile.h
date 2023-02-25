// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSProjectile.generated.h"

class USphereComponent;

UCLASS()
class MYSHOOTER_API AMSProjectile : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    USphereComponent* CollisionComponent;

public:
    AMSProjectile();

protected:
    virtual void BeginPlay() override;
};
