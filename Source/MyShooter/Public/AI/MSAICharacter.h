// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/MSCharacter.h"
#include "MSAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class MYSHOOTER_API AMSAICharacter : public AMSCharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTree;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* HealthBarWidgetComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    float HealthBarDrawDistance = 2500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (ClampMin = "0.01", ClampMax = "1"))
    float CheckHealthBarVisibilityTimeRate = 0.5f;

private:
    FTimerHandle CheckHealthBarTimer;
    float SquaredHealthBarDrawDistance;

    bool bWantsToRun = false;

public:
    AMSAICharacter(const FObjectInitializer& ObjInit);

    virtual bool IsRunning() const { return bWantsToRun && IsMovingForward(); }
    FORCEINLINE void StartRunning() { bWantsToRun = true; }
    FORCEINLINE void StopRunning() { bWantsToRun = false; }

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type Reason) override;

    virtual void OnDeath() override;
    virtual void OnHealthChanged(float NewHealth, float HealthDelta) override;

private:
    void OnCheckHealthBarVisibility();

    FORCEINLINE bool IsMovingForward() const { return FVector::DotProduct(GetVelocity(), GetActorForwardVector()) > 0.6f; }
};
