// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSPlayerNameWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTERARENA_API UMSPlayerNameWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* NameTextBlock;

public:
    void SetName(const FString& Name);

protected:
    virtual void NativeOnInitialized() override;
};

