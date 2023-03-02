// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/MSWeapon.h"
#include "Core/CoreUtils.h"
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

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const;

    virtual bool Initialize() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnDamageTaken();

private:
    template<typename T>
    FORCEINLINE T* GetPlayerComponent() const
    {
        return FCoreUtils::GetActorComponent<T>(GetOwningPlayerPawn());
    }

    void OnHealthChanged(float Health, float DeltaHealth);
};
