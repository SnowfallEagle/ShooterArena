// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/MSCharacterMovementComponent.h"
#include "MSCharacter.generated.h"

class UMSHealthComponent;
class UMSWeaponComponent;
class USoundCue;

UCLASS()
class MYSHOOTER_API AMSCharacter : public ACharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UMSHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UMSWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorName = "Paint Color";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* DeathSound;

public:
    AMSCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;

    void SetCharacterColor(const FLinearColor& Color);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsRunning() const { return false; }

protected:
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    virtual void BeginPlay() override;
    virtual void TurnOff() override;

    virtual void OnDeath();
    virtual void OnHealthChanged(float NewHealth, float HealthDelta);

private:
    UFUNCTION()
    void OnGroundLanded(const FHitResult& HitResult);
};
