// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MSAIController.generated.h"

class UMSAIPerceptionComponent;

UCLASS()
class MYSHOOTER_API AMSAIController : public AAIController
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    UMSAIPerceptionComponent* MSAIPerceptionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    FName FocusedActorKeyName = "EnemyActor";

public:
    AMSAIController();

protected:
    virtual void Tick(float DeltaTime) override;
    virtual void OnPossess(APawn* InPawn) override;

private:
    AActor* GetFocusedActor() const;
};
