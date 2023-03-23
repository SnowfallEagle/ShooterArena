// MyShooter Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSPauseWidget.generated.h"

class UButton;

UCLASS()
class MYSHOOTER_API UMSPauseWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ClearPauseButton;

public:
    virtual bool Initialize() override;

private:
    UFUNCTION()
    void OnClearPause();
};
