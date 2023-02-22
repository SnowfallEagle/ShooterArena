// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class MYSHOOTER_API AMSWeapon : public AActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

public:
    AMSWeapon();

protected:
    virtual void BeginPlay() override;
};
