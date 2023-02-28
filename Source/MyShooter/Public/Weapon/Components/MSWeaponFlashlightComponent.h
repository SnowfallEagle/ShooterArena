// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "MSWeaponFlashlightComponent.generated.h"

UCLASS()
class MYSHOOTER_API UMSWeaponFlashlightComponent : public USpotLightComponent
{
    GENERATED_BODY()

private:
    bool bEnabled = false;

public:
    UFUNCTION(BlueprintCallable)
    void ToggleFlashlight();

protected:
    virtual void BeginPlay() override;
};
