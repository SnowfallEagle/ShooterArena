// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/MSWeapon.h"
#include "Core/CoreUtils.h"
#include "MSGameModeBase.h"
#include "MSPlayerHUDWidget.generated.h"

class UProgressBar;

UCLASS()
class MYSHOOTER_API UMSPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* DamageAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float VisibilityThreshold = 0.9f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ColorThreshold = 0.25f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor HealthyColor = FLinearColor(0.5f, 0.0f, 0.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor DyingColor = FLinearColor::Red;    

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

    UFUNCTION(BlueprintCallable, Category = "UI")
    AMSGameModeBase* GetGameMode();

    UFUNCTION(BlueprintCallable, Category = "UI")
    FORCEINLINE AMSPlayerState* GetPlayerState() { return GetOwningPlayerState<AMSPlayerState>(); }

    UFUNCTION(BlueprintCallable, Category = "UI")
    FORCEINLINE FString TimerSecondsToString(int Seconds) { return FString::Printf(TEXT("%.2d:%.2d"), Seconds / 60, Seconds % 60); }

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnDamageTaken();

    virtual void NativeOnInitialized() override;

private:
    template<typename T>
    FORCEINLINE T* GetPlayerComponent() const
    {
        return FCoreUtils::GetActorComponent<T>(GetOwningPlayerPawn());
    }

    void UpdateHealthBar();

    void OnHealthChanged(float Health, float DeltaHealth);
    void OnPawnChanged(APawn* NewPawn);
};
