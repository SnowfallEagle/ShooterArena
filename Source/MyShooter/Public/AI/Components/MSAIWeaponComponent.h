// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/Components/MSWeaponComponent.h"
#include "MSAIWeaponComponent.generated.h"

UCLASS()
class MYSHOOTER_API UMSAIWeaponComponent : public UMSWeaponComponent
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void NextWeapon() override;
};
