// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "MSWeaponFlashlightComponent.generated.h"

UCLASS()
class SHOOTERARENA_API UMSWeaponFlashlightComponent : public USpotLightComponent
{
    GENERATED_BODY()

private:
    bool bEnabled = false;

public:
    FORCEINLINE void Toggle() { SetState(!bEnabled); }
    FORCEINLINE void Toggle(bool bInEnabled) { SetState(bInEnabled); }

    void OnEquipped() { SetVisibility(bEnabled, true); }
    void OnUnequipped() { SetVisibility(false, true); }

protected:
    virtual void BeginPlay() override;

private:
    void SetState(bool bInEnabled);
};
