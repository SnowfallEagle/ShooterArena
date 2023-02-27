// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/MSWeapon.h"
#include "MSPlayerHUDWidget.generated.h"

UCLASS()
class MYSHOOTER_API UMSPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponUIData(FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponAmmoData(FAmmoData& CurrentAmmo, FAmmoData& DefaultAmmo) const;

private:
    template<typename T>
    T* GetPlayerComponent() const;
};
