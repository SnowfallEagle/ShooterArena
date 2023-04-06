// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MSMenuHUD.generated.h"

UCLASS()
class SHOOTERARENA_API AMSMenuHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;

protected:
    virtual void BeginPlay() override;
};
