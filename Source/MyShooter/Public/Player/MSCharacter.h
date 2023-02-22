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

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

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

protected:
    virtual void BeginPlay() override;

private:
    void MoveForward(float Amount);
    FORCEINLINE void MoveRight(float Amount) { AddMovementInput(GetActorRightVector(), Amount); }

    FORCEINLINE void LookUp(float Amount) { AddControllerPitchInput(Amount); }
    FORCEINLINE void TurnAround(float Amount) { AddControllerYawInput(Amount); }

    FORCEINLINE void OnStartRunning() { bWantsToRun = true; }
    FORCEINLINE void OnEndRunning() { bWantsToRun = false; }

    void OnDeath();
    void OnHealthChanged(float NewHealth);
};
