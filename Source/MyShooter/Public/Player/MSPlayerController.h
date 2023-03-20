// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
};
