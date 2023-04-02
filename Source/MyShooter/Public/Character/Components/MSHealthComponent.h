// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MSHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYSHOOTER_API UMSHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0001", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal")
    bool bAutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal", meta = (EditCondition = "bAutoHeal"))
    float AutoHealUpdateTime = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal", meta = (EditCondition = "bAutoHeal"))
    float AutoHealDelayTime = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal", meta = (EditCondition = "bAutoHeal"))
    float AutoHealModifier = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShakeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    TMap<const UPhysicalMaterial*, float> DamageModifiers;

private:
    float Health = 0.0f;
    FTimerHandle AutoHealTimer;
    AController* LastDamageInstigater;

public:
    UMSHealthComponent();

    UFUNCTION(BlueprintCallable, Category = "Health")
    FORCEINLINE bool IsDead() const { return FMath::IsNearlyZero(Health); }

    UFUNCTION(BlueprintCallable, Category = "Health")
    FORCEINLINE float GetHealthPercent() const { return Health / MaxHealth; }

    FORCEINLINE float GetHealth() const { return Health; }
    FORCEINLINE bool IsHealthFull() const { return FMath::IsNearlyEqual(Health, MaxHealth); }

    FORCEINLINE AController* GetLastDamageInstigater() const { return LastDamageInstigater; }

    bool TryToAddHealth(float InHealth);

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTakePointDamage(
        AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent,
        FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser
    );

    UFUNCTION()
    void OnTakeRadialDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo,
        class AController* InstigatedBy, AActor* DamageCauser
    );

private:
    void SetHealth(float InHealth);
    void OnAutoHealUpdateTimerFired();

    void PlayCameraShake();
    void ApplyDamage(float Damage, AController* Instigater);
    void ReportDamageEvent(float Damage);
};
