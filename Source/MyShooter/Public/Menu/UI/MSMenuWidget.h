// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSMenuWidget.generated.h"

class UButton;

UCLASS()
class MYSHOOTER_API UMSMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* StartButton;

protected:
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnStart();
};
