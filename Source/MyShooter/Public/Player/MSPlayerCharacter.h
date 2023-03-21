// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/MSCharacter.h"
#include "MSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MYSHOOTER_API AMSPlayerCharacter : public AMSCharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

private:
    bool bWantsToRun = false;
    bool bMovingForward = true;

public:
    AMSPlayerCharacter(const FObjectInitializer& ObjInit);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual bool IsRunning() const override { return bWantsToRun && bMovingForward && !GetVelocity().IsZero(); }

protected:

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

private:
    void MoveForward(float Amount);
    FORCEINLINE void MoveRight(float Amount) { AddMovementInput(GetActorRightVector(), Amount); }

    FORCEINLINE void LookUp(float Amount) { AddControllerPitchInput(Amount); }
    FORCEINLINE void TurnAround(float Amount) { AddControllerYawInput(Amount); }

    FORCEINLINE void OnStartRunning() { bWantsToRun = true; }
    FORCEINLINE void OnEndRunning() { bWantsToRun = false; }
};
