// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MSGameModeBase.h"
#include "MSPlayerController.generated.h"

class UMSRespawnComponent;

UCLASS()
class MYSHOOTER_API AMSPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    UMSRespawnComponent* MSRespawnComponent;

public:
    AMSPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    void OnPause();
    void OnMatchStateChanged(EMatchState NewState);
};
