// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/MSCharacterMovementComponent.h"
#include "MSCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UMSHealthComponent;
class UMSWeaponComponent;

UCLASS()
class MYSHOOTER_API AMSCharacter : public ACharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UMSHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

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

private:
    bool bWantsToRun = false;
    bool bMovingForward = true;

public:
    AMSCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const { return bWantsToRun && bMovingForward && !GetVelocity().IsZero(); }

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    void SetCharacterColor(const FLinearColor& Color);

protected:
    virtual void BeginPlay() override;
    virtual void TurnOff() override;
    virtual void OnDeath();

private:
    void MoveForward(float Amount);
    FORCEINLINE void MoveRight(float Amount) { AddMovementInput(GetActorRightVector(), Amount); }

    FORCEINLINE void LookUp(float Amount) { AddControllerPitchInput(Amount); }
    FORCEINLINE void TurnAround(float Amount) { AddControllerYawInput(Amount); }

    FORCEINLINE void OnStartRunning() { bWantsToRun = true; }
    FORCEINLINE void OnEndRunning() { bWantsToRun = false; }

    void OnHealthChanged(float NewHealth, float HealthDelta);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& HitResult);
};
