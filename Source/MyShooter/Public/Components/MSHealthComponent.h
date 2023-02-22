// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MSHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYSHOOTER_API UMSHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal")
    bool bAutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal", meta = (EditCondition = "bAutoHeal"))
    float AutoHealUpdateTime = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal", meta = (EditCondition = "bAutoHeal"))
    float AutoHealDelayTime = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AutoHeal", meta = (EditCondition = "bAutoHeal"))
    float AutoHealModifier = 1.0f;

private:
    float Health = 0.0f;

    FTimerHandle AutoHealTimer;

public:
    UMSHealthComponent();

    FORCEINLINE float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable)
    FORCEINLINE bool IsDead() const { return FMath::IsNearlyZero(Health); }

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
    void SetHealth(float InHealth);

    void OnAutoHealUpdateTimerFired();
};
