// ShooterArena Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MSHealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class SHOOTERARENA_API UMSHealthBarWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float VisibilityThreshold = 0.8f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float ColorThreshold = 0.25f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor HealthyColor = FLinearColor(0.5f, 0.0f, 0.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor DyingColor = FLinearColor::Red;

public:
    void SetHealthPercent(float Health);
};
