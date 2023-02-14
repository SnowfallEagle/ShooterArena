// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MSCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MYSHOOTER_API AMSCharacter : public ACharacter
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
    bool bRunning = false;

public:
    // Sets default values for this character's properties
    AMSCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    FORCEINLINE void MoveForward(float Amount) { AddMovementInput(GetActorForwardVector(), Amount); }
    FORCEINLINE void MoveRight(float Amount) { AddMovementInput(GetActorRightVector(), Amount); }

    FORCEINLINE void LookUp(float Amount) { AddControllerPitchInput(Amount); }
    FORCEINLINE void TurnAround(float Amount) { AddControllerYawInput(Amount); }

    FORCEINLINE void Run(float Amount) { bRunning = Amount > 0.0f; }
};
