// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/MSCharacter.h"
#include "MSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class USoundCue;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitSignature, bool /* bKilled */);

UCLASS()
class SHOOTERARENA_API AMSPlayerCharacter : public AMSCharacter
{
    GENERATED_BODY()

public:
    FOnHitSignature OnHit;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* CameraCollisionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* HitSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* DeathHitSound;

private:
    bool bWantsToRun = false;
    bool bMovingForward = true;

public:
    AMSPlayerCharacter(const FObjectInitializer& ObjInit);

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual bool IsRunning() const override { return bWantsToRun && bMovingForward && !GetVelocity().IsZero(); }

    void ReportHit(bool bKilled);

protected:
    virtual void BeginPlay() override;
    virtual void OnDeath() override;

private:
    void MoveForward(float Amount);
    FORCEINLINE void MoveRight(float Amount) { AddMovementInput(GetActorRightVector(), Amount); }

    FORCEINLINE void LookUp(float Amount) { AddControllerPitchInput(Amount); }
    FORCEINLINE void TurnAround(float Amount) { AddControllerYawInput(Amount); }

    FORCEINLINE void OnStartRunning() { bWantsToRun = true; }
    FORCEINLINE void OnEndRunning() { bWantsToRun = false; }

    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void OnCameraOverlap();
};
