// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MSWeaponComponent.generated.h"

class AMSWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYSHOOTER_API UMSWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AMSWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponAttachPointName = "WeaponSocket";

private:
    UPROPERTY()
    AMSWeapon* CurrentWeapon = nullptr;

public:
    UMSWeaponComponent();

    void Fire();

protected:
    virtual void BeginPlay() override;

private:
    void SpawnWeapon();
};
